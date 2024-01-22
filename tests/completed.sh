DK="/tmp/DerKern"
cd completed
mkdir $DK&>/dev/null
echo standlo:
g++ standlo.cpp -o $DK -Wno-return-type &&$DK

echo&&echo ascii:
g++ ascii.cpp -o $DK -Wno-return-type &&$DK

echo&&echo type:
g++ type.cpp -o $DK -Wno-return-type &&$DK

echo&&echo instructions:
g++ instructions.cpp -o $DK -Wno-return-type &&$DK
rm $DK&>/dev/null