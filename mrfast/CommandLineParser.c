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
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include "Common.h"
#include "CommandLineParser.h"

int						uniqueMode=1;
int						indexingMode;
int						searchingMode;
int						pairedEndMode;
int						pairedEndDiscordantMode;
int						transChromosal=0;
int						pairedEndProfilingMode;
int						seqCompressed;
int						outCompressed;
int						cropSize = 0;
int						progressRep = 0;
int						minPairEndedDistance=-1;
int						maxPairEndedDistance=-1;
int						minPairEndedDiscordantDistance=-1;
int						maxPairEndedDiscordantDistance=-1;
int 					bestMode;
int 					nosamMode;
char					*seqFile1;
char					*seqFile2;
char					*mappingOutput = "output";
char					*mappingOutputPath = "";
char					*unmappedOutput = "unmapped";
char					fileName[1000][2][FILE_NAME_LENGTH];
int						fileCnt;
int 					maxOEAOutput=100;
int 					maxDiscordantOutput=300;
unsigned char			errThreshold=2;
unsigned char			maxHits=0;
unsigned char			WINDOW_SIZE = 12;
unsigned int			CONTIG_SIZE;
unsigned int			CONTIG_MAX_SIZE;

void printHelp();

int parseCommandLine (int argc, char *argv[])
{

	int o;
	int index;
	char *fastaFile = NULL;
	char *batchFile = NULL ;
	int  batchMode = 0;
		
	static struct option longOptions[] = 
	{
		{"pe",				no_argument,		&pairedEndMode,		1},
		{"discordant-vh",	        no_argument,		&pairedEndDiscordantMode,	1},
		{"trans",                     no_argument,            &transChromosal        ,       1},
		{"profile",			no_argument, 		&pairedEndProfilingMode,	1},
		{"seqcomp",			no_argument,		&seqCompressed,		1},
		{"outcomp",			no_argument,		&outCompressed,		1},
		{"progress",		no_argument,		&progressRep,		1},
		{"best",		no_argument,		&bestMode,		1},
		{"index",			required_argument,	0, 					'i'},
		{"search",			required_argument,	0,					's'},
		{"help",			no_argument,		0,					'h'},
		{"version",			no_argument,		0,					'v'},
		{"seq",				required_argument,	0,					'x'},
		{"seq1",			required_argument,	0,					'x'},
		{"seq2",			required_argument,	0,					'y'},
		{"ws",				required_argument,  0,					'w'},
		{"min",				required_argument,  0,					'l'},
		{"max",				required_argument,  0,					'm'},
		{"crop",			required_argument,  0,					'c'},
		{"maxoea",            required_argument,  0,                  'a'},
		{"maxdis",            required_argument,  0,                  'd'},
		{"nosam",        no_argument,        &nosamMode,      1},
		{0,  0,  0, 0},
	};

	while ( (o = getopt_long ( argc, argv, "bhvn:e:o:u:i:s:x:y:w:l:m:c:a:d:", longOptions, &index)) != -1 )
	{
		switch (o)
		{
			case 'a':
				maxOEAOutput = atoi(optarg);
				if (maxOEAOutput == 0)
				  maxOEAOutput = 100000;
				break;
			case 'd':
				maxDiscordantOutput = atoi(optarg);
				if (maxDiscordantOutput == 0)
				  maxDiscordantOutput = 100000;
				break;
			case 'i':
				indexingMode = 1;
				fastaFile = optarg;
				break;
			case 's':
				searchingMode = 1;
				fastaFile = optarg;
				break;
			case 'b':
				batchMode = 1;
				break;
			case 'c': 
				cropSize = atoi(optarg);
				break;
			case 'w':
				WINDOW_SIZE = atoi(optarg);
				break;
			case 'x':
				seqFile1 = optarg;
				break;
			case 'y':
				seqFile2 = optarg;
				break;
			case 'u':
				unmappedOutput = optarg;
				break;
			case 'o':
				mappingOutput = getMem(FILE_NAME_LENGTH);
				mappingOutputPath = getMem(FILE_NAME_LENGTH);
				stripPath (optarg, &mappingOutputPath, &mappingOutput);
				break;
			case 'n':
				maxHits = atoi(optarg);
				break;
			case 'e':
				errThreshold = atoi(optarg);
				break;
			case 'l':
				minPairEndedDistance = atoi(optarg);
				break;
			case 'm':
				maxPairEndedDistance = atoi(optarg);
				break;					
			case 'h':
				printHelp();
				return 0;
				break;
			case 'v':
				fprintf(stdout, "%s.%s\n", versionNumber, versionNumberF);
				return 0;
				break;
				/*		        case '?':
 			        fprintf(stderr, "Unknown parameter: %s\n", longOptions[index].name);
			        abort();
				break;*/
		}

	}
	if (indexingMode + searchingMode != 1)
	{
		fprintf(stdout, "ERROR: Indexing / Searching mode should be selected\n");
		return 0;
	}

	if (WINDOW_SIZE > 15 || WINDOW_SIZE < 11)
	{
		fprintf(stdout, "ERROR: Window size should be in [12..15]\n");
		return 0;
	}


	if ( indexingMode )
	{
		CONTIG_SIZE		= 15000000*20;	// fastHASH
		CONTIG_MAX_SIZE	= 40000000*20;	// fastHASH

		if (batchMode)
		{
			batchFile = fastaFile;
			fastaFile = NULL;
		}

		if (batchFile == NULL && fastaFile == NULL)
		{
			fprintf(stdout, "ERROR: Reference(s) should be indicated for indexing\n");
			return 0;
		}

		if (pairedEndDiscordantMode)
		{
			fprintf(stdout, "ERROR: --discordant-vh cannot be used in indexing mode. \n");
			return 0;
		}

	}


	if ( searchingMode )
	{
		CONTIG_SIZE		= 300000000;
		CONTIG_MAX_SIZE	= 300000000;


		if (batchMode)
		{
			batchFile = fastaFile;
			fastaFile = NULL;
		}

		if (batchFile == NULL && fastaFile == NULL)
		{
			fprintf(stdout, "ERROR: Index File(s) should be indiciated for searching\n");
			return 0;
		}

		if (seqFile1 == NULL && seqFile2 == NULL)
		{
			fprintf(stdout, "ERROR: Please indicate a sequence file for searching.\n");
			return 0;
		}


		if (!pairedEndMode && seqFile2 != NULL)
		{
			fprintf(stdout, "ERROR: Second File can be indicated in pairedend mode\n");
			return 0;
		}

		if (pairedEndMode && (minPairEndedDistance <0 || maxPairEndedDistance < 0 || minPairEndedDistance > maxPairEndedDistance))
		{
			fprintf(stdout, "ERROR: Please enter a valid range for pairedend sequences.\n");
			return 0;
		}

		if (pairedEndMode && seqFile1 == NULL)
		{
			fprintf(stdout, "ERROR: Please indicate the first file for pairedend search.\n");
			return 0;
		}

		if (!pairedEndMode && pairedEndDiscordantMode)
		{
			fprintf(stdout, "ERROR: --discordant should be used with --pe");
			return 0;
		}

		if (!pairedEndMode && pairedEndProfilingMode)
		{
			fprintf(stdout, "ERROR: --profile should be used with --pe");
			return 0;
		}

		if (pairedEndMode)
		        pairedEndDiscordantMode = 1;
	}

	int i = 0;


	if (batchMode)
	{
		FILE *fp = fileOpen(batchFile, "r");

		if (fp == NULL)
			return 0;

		fileCnt  = 0;

		while ( fgets(fileName[fileCnt][0], FILE_NAME_LENGTH, fp))
		{
			for (i = strlen(fileName[fileCnt][0])-1; i>=0; i--)
				if ( !isspace(fileName[fileCnt][0][i]))
					break;
			fileName[fileCnt][0][i+1] = '\0';

			if (strcmp(fileName[fileCnt][0], "") != 0)
			{
				sprintf(fileName[fileCnt][1], "%s.index", fileName[fileCnt][0]); 
				fileCnt++;
			}
		}
	}
	else
	{
		sprintf(fileName[fileCnt][0], "%s", fastaFile);
		sprintf(fileName[fileCnt][1], "%s.index", fileName[fileCnt][0]); 
		fileCnt++;
	}


	if (pairedEndProfilingMode)
	{

		minPairEndedDistance = 0;
		maxPairEndedDistance = 300000000;

	}

	if (pairedEndDiscordantMode)
	{
		minPairEndedDiscordantDistance = minPairEndedDistance;
		maxPairEndedDiscordantDistance = maxPairEndedDistance;

		minPairEndedDistance = 0;
		maxPairEndedDistance = 300000000;
	}

	return 1;
}


void printHelp()
{
	char *errorType;
	if (mrFAST)
	{
		fprintf(stdout,"mrFAST : Micro-Read Fast Alignment Search Tool.\n\n");
		fprintf(stdout,"Usage: mrfast [options]\n\n");
		errorType="edit distance";
	}
	else
	{
		fprintf(stdout,"mrsFAST : Micro-Read Substitutions (only) Fast Alignment Search Tool.\n\n");
		fprintf(stdout,"mrsFAST is a cache oblivious read mapping tool. mrsFAST capable of mapping\n");
		fprintf(stdout,"single and paired end reads to the reference genome. Bisulfite treated \n");
		fprintf(stdout,"sequences are not supported in this version. By default mrsFAST reports  \n");
		fprintf(stdout,"the output in SAM format.\n\n");
		fprintf(stdout,"Usage: mrsFAST [options]\n\n");
		errorType="hamming distance";
	}

	fprintf(stdout,"General Options:\n");
	fprintf(stdout," -v|--version\t\tCurrent Version.\n");
	fprintf(stdout," -h\t\t\tShows the help file.\n");
	fprintf(stdout,"\n\n");

	fprintf(stdout,"Indexing Options:\n");
	fprintf(stdout," --index [file]\t\tGenerate an index from the specified fasta file. \n");
	fprintf(stdout," -b\t\t\tIndicates the indexing will be done in batch mode.\n\t\t\tThe file specified in --index should contain the \n\t\t\tlist of fasta files.\n");
	fprintf(stdout," --ws [int]\t\tSet window size for indexing (default:12 max:14).\n");
	fprintf(stdout,"\n\n");

	fprintf(stdout,"Searching Options:\n");
	fprintf(stdout," --search [file]\tSearch in the specified genome. Provide the path to the fasta file. \n\t\t\tIndex file should be in the same directory.\n");
	fprintf(stdout," -b\t\t\tIndicates the mapping will be done in batch mode. \n\t\t\tThe file specified in --search should contain the \n\t\t\tlist of fasta files.\n");
	fprintf(stdout," --pe \t\t\tSearch will be done in Paired-End mode.\n");
	fprintf(stdout," --seq [file]\t\tInput sequences in fasta/fastq format [file]. If \n\t\t\tpaired end reads are interleaved, use this option.\n");
	fprintf(stdout," --seq1 [file]\t\tInput sequences in fasta/fastq format [file] (First \n\t\t\tfile). Use this option to indicate the first file of \n\t\t\tpaired end reads. \n");
	fprintf(stdout," --seq2 [file]\t\tInput sequences in fasta/fastq format [file] (Second \n\t\t\tfile). Use this option to indicate the second file of \n\t\t\tpaired end reads.  \n");
	fprintf(stdout," -o [file]\t\tOutput of the mapped sequences. The default is \"output\".\n");
	fprintf(stdout," -u [file]\t\tSave unmapped sequences in fasta/fastq format.\n");
        fprintf(stdout," --best   \t\tOnly the best mapping from all the possible mapping is returned.\n");
	fprintf(stdout," --seqcomp \t\tIndicates that the input sequences are compressed (gz).\n");
	fprintf(stdout," --outcomp \t\tIndicates that output file should be compressed (gz).\n");
	fprintf(stdout," -e [int]\t\tMaximum allowed %s (default 2).\n", errorType);
	fprintf(stdout," --min [int]\t\tMin distance allowed between a pair of end sequences.\n");
	fprintf(stdout," --max [int]\t\tMax distance allowed between a pair of end sequences.\n");

	fprintf(stdout," --maxoea [int]\t\tMax number of One End Anchored (OEA) returned for each read pair. We recommend 100 or above for NovelSeq use.\n");
	fprintf(stdout," --maxdis [int]\t\tMax number of discordant map locations returned for each read pair. We recommend 300 or above for VariationHunter use.\n");
}
