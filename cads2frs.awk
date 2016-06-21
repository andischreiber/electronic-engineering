#!/usr/bin/awk -f
#
# Entfernt ", und ergänzt .PAC-Zeile mit Decalname
# (es darf kein Leerzeichen im Decalname vorkommen)
#
# Autor: Andreas Schreiber
# Date: 02.09.2008
#
# Aufruf:
# awk -f cads2frs.awk inputfile.frs >outputfile.frs
#

BEGIN {}

$1 == ".ADD_COM" {
    gsub(/\"/, "");
    print $1 " " $2 " " $4;
    printf(".PAC %s\n", $3);
    printf "\n";
    next;
    }

$1 == ".ATT_COM" || $1 ==".ATT_TRE" {
    next;
    }

$1 == ".ADD_TER" {
    gsub(/\"/, "");
    }

{
    print $0;
    }

END {} 
