#!/usr/bin/awk
BEGIN {
    print "Enter bye when you are done."
}


   { 
   print "You entered: " $0
   }

/bye/  {
       exit;
       }



