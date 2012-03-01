/*
 * Copyright (c) <2008 - 2012>, University of Washington, Simon Fraser University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or other
 *   materials provided with the distribution.
 * - Neither the names of the University of Washington, Simon Fraser University, 
 *   nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
  Authors: 
      Farhad Hormozdiari
      Faraz Hach
      Can Alkan
  Emails:
      farhadh AT uw DOT edu
      fhach AT cs DOT sfu DOT ca
      calkan AT uw DOT edu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Common.h"
#include "CommandLineParser.h"
#include "Reads.h"
#include "Output.h"
#include "HashTable.h"
#include "MrFAST.h"
#include "bit_mask.h"

char 				*versionNumber = "2.1";			// Current Version
unsigned char		seqFastq;

int main(int argc, char *argv[])
{
	int search_key_level[7];
	int it;
	for (it = 0; it < 7; it++) {
		search_key_level[it] = 0;
	}

	if (!parseCommandLine(argc, argv))
		return 1;

	configHashTable();
	/****************************************************
	 * INDEXING
	 ***************************************************/
	if (indexingMode)
	{
		int i;
		/********************************
		 * Regular Mode
		 ********************************/
		configHashTable();
		for (i = 0; i < fileCnt; i++)
		{
			generateHashTable(fileName[i][0], fileName[i][1]);
		}
	}
	/****************************************************
	 * SEARCHING
	 ***************************************************/
	else
	{
		Read *seqList;
		unsigned int seqListSize;
		int fc;
		int samplingLocsSize;
		int *samplingLocs;
		double totalLoadingTime = 0;
		double totalMappingTime = 0;
		double startTime;
		double loadingTime;
		double mappingTime;
		double lstartTime;
		double ppTime = 0.0;
		double tmpTime;;
		double BitMaskTime;;
		char *prevGen = getMem(CONTIG_NAME_SIZE);
		prevGen[0]='\0';
		char *curGen;
		int	flag;
		double maxMem=0;
		char fname1[FILE_NAME_LENGTH];
		char fname2[FILE_NAME_LENGTH];
		char fname3[FILE_NAME_LENGTH];
		char fname4[FILE_NAME_LENGTH];
		char fname5[FILE_NAME_LENGTH];
	
		char outputFileName[FILE_NAME_LENGTH];
		// Loading Sequences & Sampling Locations
		startTime = getTime();
		if (!readAllReads(seqFile1, seqFile2, seqCompressed, &seqFastq, pairedEndMode, &seqList, &seqListSize))
		{
			return 1;
		}

		loadSamplingLocations(&samplingLocs, &samplingLocsSize);
		totalLoadingTime += getTime()-startTime;

		if (pairedEndMode)
		{
			//Switching to Inferred Size 
			minPairEndedDistance = minPairEndedDistance - SEQ_LENGTH + 2;
			maxPairEndedDistance = maxPairEndedDistance - SEQ_LENGTH + 2;
			if (pairedEndDiscordantMode)
			{
				maxPairEndedDiscordantDistance = maxPairEndedDiscordantDistance - SEQ_LENGTH + 2;
				minPairEndedDiscordantDistance = minPairEndedDiscordantDistance - SEQ_LENGTH + 2;
			}
			
			sprintf(fname1, "__%s__1", mappingOutput);
			sprintf(fname2, "__%s__2", mappingOutput);
			sprintf(fname3, "__%s__disc", mappingOutput);
			sprintf(fname4, "__%s__oea1", mappingOutput);
			sprintf(fname5, "__%s__oea2", mappingOutput);
			unlink(fname1);
			unlink(fname2);
			unlink(fname3);
			unlink(fname4);
			unlink(fname5);
		}

		sprintf(outputFileName, "%s%s",mappingOutputPath , mappingOutput);
		// Preparing output
		initOutput(outputFileName, outCompressed);

		fprintf(stdout, "-----------------------------------------------------------------------------------------------------------\n");
		fprintf(stdout, "| %15s | %15s | %15s | %15s | %15s %15s |\n","Genome Name","Loading Time", "Mapping Time", "Memory Usage(M)","Total Mappings","Mapped reads");
		fprintf(stdout, "-----------------------------------------------------------------------------------------------------------\n");

		/********************************
		 * Regular Mode
		 ********************************/
			if (!pairedEndMode)
			{
				initLookUpTable();
				if(bestMode)
                                        initBestMapping(seqListSize);

				for (fc = 0; fc <fileCnt; fc++)
				{
					if (!initLoadingHashTable(fileName[fc][1]))
					{
						return 1;
					}
							
					mappingTime = 0;
					loadingTime = 0;
					prevGen[0] = '\0';
					flag = 1;
					int hash_file_num = 0;
					do
					{
						flag = loadHashTable(&tmpTime, errThreshold);  			// Reading a fragment
						curGen = getRefGenomeName();
						if (flag) {
							load_mask(hash_file_num, &BitMaskTime);								// DHL bit mask
							hash_file_num = hash_file_num + 1;
						}

						// First Time
						if (flag && prevGen[0]== '\0')
						{
							sprintf(prevGen, "%s", curGen);
						}

						if ( !flag || strcmp(prevGen, curGen)!=0)
						{

							fprintf(stdout, "| %15s | %15.2f | %15.2f | %15.2f | %15lld %15lld |\n",
									prevGen,loadingTime, mappingTime, maxMem, mappingCnt , mappedSeqCnt);
							int iq;
							for (iq = 0; iq < 7; iq++) {
								fprintf(stdout, "%d level bit mask success: %lld \n",
										iq+1, get_eval_data(1, iq));
								fprintf(stdout, "%d level bit mask fail   : %lld \n",
										iq+1, get_eval_data(0, iq));
							}
							fprintf(stdout,"---------------------------------------------\n");
							fprintf(stdout,"Total Filtering       : %lld \n", total_filtering);
							fprintf(stdout,"Total Binary num	  : %lld \n", total_binary_num);
							fprintf(stdout,"Total binary success  : %lld \n", total_binary_success);
							fprintf(stdout,"Total bit_mask success: %lld \n", total_bit_mask_success);
							fprintf(stdout,"Total edit num		  : %lld \n", total_edit_num);
							fprintf(stdout,"Total pass num		  : %lld \n", total_pass_num);
							fprintf(stdout,"---------------------------------------------\n");
							bitmask_init();

							fflush(stdout);
							

							totalMappingTime += mappingTime;
							totalLoadingTime += loadingTime;

							loadingTime = 0;
							mappingTime = 0;
							maxMem = 0;

							if (!flag)
							{
								break;
							}
						}
						else if (progressRep && mappingTime != 0)
						{
							fprintf(stdout, "| %15s | %15.2f | %15.2f | %15.2f | %15lld %15lld |\n",
									prevGen,loadingTime, mappingTime, maxMem, mappingCnt , mappedSeqCnt);
							int iq;
							for (iq = 0; iq < 7; iq++) {
								fprintf(stdout, "%d level bit mask success: %lld \n",
										iq+1, get_eval_data(1, iq));
								fprintf(stdout, "%d level bit mask fail   : %lld \n",
										iq+1, get_eval_data(0, iq));
							}
							fprintf(stdout,"---------------------------------------------\n");
							fprintf(stdout,"Total Filtering       : %lld \n", total_filtering);
							fprintf(stdout,"Total Binary num	  : %lld \n", total_binary_num);
							fprintf(stdout,"Total binary success  : %lld \n", total_binary_success);
							fprintf(stdout,"Total bit_mask success: %lld \n", total_bit_mask_success);
							fprintf(stdout,"Total edit num		  : %lld \n", total_edit_num);
							fprintf(stdout,"Total pass num		  : %lld \n", total_pass_num);
							fprintf(stdout,"---------------------------------------------\n");
							bitmask_init();

							fflush(stdout);
						}

						sprintf(prevGen, "%s", curGen);

						loadingTime += tmpTime;
						loadingTime += BitMaskTime;
//						lstartTime = getTime();

						initFAST(seqList, seqListSize, samplingLocs, samplingLocsSize, fileName[fc][0]);

						lstartTime = getTime();

						mapAllSingleEndSeq(); // DHL
						//mapAllSingleEndSeq(flag_first); // DHL
						
						mappingTime += getTime() - lstartTime;
						if (maxMem < getMemUsage())
						{
							maxMem = getMemUsage();					 
						}
						if (flag) {
							free_mask();
						}
					} while (flag);

				} // end for;
				if(bestMode)
					finalizeBestSingleMapping();
				finalizeFAST();
				finalizeLoadingHashTable();

			}
			// Pairedend Mapping Mode
			else
			{
				initLookUpTable();
				if(pairedEndMode)
					initBestConcordantDiscordant(seqListSize);
				for (fc = 0; fc < fileCnt; fc++)
				{
					if (!initLoadingHashTable(fileName[fc][1]))
					{
						return 1;
					}
					mappingTime = 0;
					loadingTime = 0;
					prevGen[0] = '\0';
					flag = 1;

					do
					{
						flag = loadHashTable ( &tmpTime , errThreshold);  			// Reading a fragment
						curGen = getRefGenomeName();

						// First Time
						if (flag && prevGen[0]== '\0')
						{
							sprintf(prevGen, "%s", curGen);
						}

						if ( !flag || strcmp(prevGen, curGen)!=0)
						{

							// DISCORDANT
							lstartTime = getTime();					
							outputPairedEnd();
							mappingTime += getTime() - lstartTime;
							//DISCORDANT			

							fprintf(stdout, "| %15s | %15.2f | %15.2f | %15.2f | %15lld %15lld |\n",
									prevGen,loadingTime, mappingTime, maxMem, mappingCnt , mappedSeqCnt);
							fflush(stdout);

							totalMappingTime += mappingTime;
							totalLoadingTime += loadingTime;

							loadingTime = 0;
							mappingTime = 0;
							maxMem = 0;

							if (!flag)
							{
								break;
							}
						}
						else if (progressRep && mappingTime != 0)
						{
							fprintf(stdout, "| %15s | %15.2f | %15.2f | %15.2f | %15lld %15lld |\n",
									prevGen,loadingTime, mappingTime, maxMem, mappingCnt , mappedSeqCnt);
							fflush(stdout);
						}

						sprintf(prevGen, "%s", curGen);

						loadingTime += tmpTime;
						lstartTime = getTime();
					
						initFAST(seqList, seqListSize, samplingLocs, samplingLocsSize, fileName[fc][0]);
						
						mapPairedEndSeq();
					
						mappingTime += getTime() - lstartTime;
						if (maxMem < getMemUsage())
						{
							maxMem = getMemUsage();
						}

					} while (flag);

				} // end for;
				
				if(pairedEndMode)
				{		
					sprintf(outputFileName, "%s%s",mappingOutputPath , mappingOutput);		
					finalizeOEAReads(outputFileName);			
					outputAllTransChromosal(transChromosal);
					finalizeBestConcordantDiscordant();
				}
			
				finalizeLoadingHashTable();
				
				if (pairedEndDiscordantMode)
				{
					lstartTime = getTime();							
					outputPairedEndDiscPP();
					ppTime = getTime() - lstartTime;
				}
				finalizeFAST();
			} //else

		finalizeOutput();

		fprintf(stdout, "-----------------------------------------------------------------------------------------------------------\n");
		fprintf(stdout, "%19s%16.2f%18.2f\n\n", "Total:",totalLoadingTime, totalMappingTime);
		if (pairedEndDiscordantMode)
			fprintf(stdout, "Post Processing Time: %18.2f \n", ppTime);
		fprintf(stdout, "%-30s%10.2f\n","Total Time:", totalMappingTime+totalLoadingTime);
		fprintf(stdout, "%-30s%10d\n","Total No. of Reads:", seqListSize);
		fprintf(stdout, "%-30s%10lld\n","Total No. of Mappings:", mappingCnt);
		fprintf(stdout, "%-30s%10.0f\n\n","Avg No. of locations verified:", ceil((float)verificationCnt/seqListSize));

		int cof = (pairedEndMode)?2:1;

		if (progressRep && maxHits != 0)
		{
			int frequency[maxHits+1];
			int i;
			for ( i=0 ; i <= maxHits; i++)
			{
				frequency[i] = 0;
			}


			for (fc = 0; fc < seqListSize; fc++)
			{
			  frequency[(int)(*(seqList[fc*cof].hits))]++;
			}
			frequency[maxHits] = completedSeqCnt;
			for ( i=0 ; i <= maxHits; i++)
			{
				fprintf(stdout, "%-30s%10d%10d%10.2f%%\n","Reads Mapped to ", i, frequency[i], 100*(float)frequency[i]/(float)seqListSize);
			}
		}


		finalizeReads(unmappedOutput);
		freeMem(prevGen, CONTIG_NAME_SIZE);
	}

	return 1;
}
