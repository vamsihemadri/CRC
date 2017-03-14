# CRC
a simple stop and wait logical data link channel between a client and a server using socket api with CRC error deterction, with the generator polynomial being 100000111 (x^8+x^2+x+1).
DONE:
concurrent server.
message converted into bits before sending.
implement soft closing of ports in server.(graceful release of open socket.)
implemented CRC.

 TO DO:
 implement inclusion of errors .
 implement timer in client.
