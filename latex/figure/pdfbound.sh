#!/bin/bash
# Example: ./pdfbound.sh figure.pdf

if [ $# != 1 ]; then
  echo "Usage: pdfbound.sh SOME_FIGURE.pdf"
  exit 1
fi

PDF_F=$1
PS_F=`echo $PDF_F | sed "s/\(.*\)\.pdf/\1\.ps/"`
EPS_F=`echo $PDF_F | sed "s/\(.*\)\.pdf/\1\.eps/"`
EPS_BOUND_F=`echo $PDF_F | sed "s/\(.*\)\.pdf/\1_B\.eps/"`
PDF_BOUND_F=`echo $PDF_F | sed "s/\(.*\)\.pdf/\1_B\.pdf/"`

pdf2ps $PDF_F

ps2eps -f -B $PS_F
rm $PS_F

mv -f $EPS_F $EPS_BOUND_F

epstopdf $EPS_BOUND_F
rm $EPS_BOUND_F
