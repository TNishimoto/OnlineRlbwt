
#!/bin/bash
cd `dirname $0`

INPUTDIR=../data/external/repcorpus
OUTPUTDIR=../data/bwt
#INPUTDIR=/Users/nishimoto/Documents/test_data/pizza
#OUTPUTDIR=/Users/nishimoto/Documents/test_data/bwt

SCRIPT_DIR=$(cd $(dirname $0); pwd)
echo "Script directory: $SCRIPT_DIR"


for file in $INPUTDIR/*; do
    if [ -f "$file" ]; then
        echo "Full path: $(realpath "$file")"
        echo "File name: $(basename "$file")"
      $SCRIPT_DIR/../build/online_bwt.out -i $INPUTDIR/$(basename "$file") -o $OUTPUTDIR/$(basename "$file").bwt
    fi
done

