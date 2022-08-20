openssl s_client -starttls smtp -connect smtp.office365.com:587 -crlf -ign_eof

openssl s_client -starttls smtp -connect smtp.gmail.com:587 -crlf -ign_eof





arm-himix200-linux-gcc -DSMTP_OpenSSL smtp.c -c -o smtp.o;

arm-himix200-linux-gcc -DSMTP_OPENSSL test.c -c -o test.o;

arm-himix200-linux-gcc test.o smtp.o -o smtp_test -L./lib -lssl -lcrypto;

/bin/cp -rf smtp_test /nfs/openssl/smtp_test

