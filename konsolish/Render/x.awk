BEGIN {
}

/^ENCODING/ { enc = $2; }

/^ENDCHAR/ { list = 0; printf(" } },\n"); next; }
/^BITMAP/ { list = 1; printf("  { %5d, {",enc); next; }
 { if (list) printf(" 0x%s,",$1); }

END {
}
