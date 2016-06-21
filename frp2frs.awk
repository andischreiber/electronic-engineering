#!/usr/bin/awk -f
#
# Autor: Andreas Schreiber
# Date: 02.09.2008
#
# Aufruf:
# awk -f frp2frs.awk inputfile.fpr >outputfile.frs

BEGIN {}

$1 == ".ATT_COM" && $3 ~ /Footprint/ {
    gsub(/\"/, "");
    printf(".PAC %s\n", $4); 
    next;
}

$1 == ".ATT_COM" || $1 ==".ATT_TRE" {
    next;
    }

$1 == ".ADD_COM" {
    print $1 " " $2 " " "dummy1";
    next;
    }

$1 == ".ADD_TER" {
    gsub(/\"/, "");
    }

{
    print $0;
    }

END {} 
