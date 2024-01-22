DK="/tmp/DerKern"
# mkdir $DK&>/dev/null
# mkdir $DK/$(dirname $1)&>/dev/null
fil=$1
shift
g++ $fil.cpp -o $DK -Wno-return-type $@ &&$DK
rm $DK&>/dev/null
# g++ $fil.cpp -o $DK/$fil -Wno-return-type $@ &&$DK/$fil