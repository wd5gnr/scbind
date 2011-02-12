#!/bin/bash
awk "`cat <<"EOF"
   { 
   print "You entered: " $0
   }

/bye/  {
       exit;
       }
EOF
`"


