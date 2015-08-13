#include    <stdlib.h>
#include    <stdio.h>
#include    <io.h>
#include    <SYS\Stat.h>
#include    <share.h>
#include    <conio.h>
#include    <fcntl.h>
//
//  This utility is designed to display checksum in byte
//
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
#define SIZE_BUFFER	  0x100


void ShowUsage(void)
{
  printf("This tool show file checksum by byte.\n\n"
    "Usage:  Checksum [Dest File] [/d] \n"
            "\t/d:Double Checksum\n"
   "\n");
}

main(int argc, char *argv[])
{
  int fhSrc;
  int nDoubleWordChecksum = 0;
  int i;
  BYTE bBuffer[SIZE_BUFFER];
  DWORD Checksum;
  DWORD lCurrentOffset;
  DWORD lROMSize = 0;
  char option_char;

  if (argc<2) {
    ShowUsage();
    exit(-1);
  }
  //
  // processing parameters
  //
  if ( argc>1)
  {
    for (i=1;i <argc; i++)
    {
      if ((*argv[i] == '/') || (*argv[i] == '-'))
      {
        option_char = (char)(*(argv[i]+1));
        switch (option_char)
        {
        case 'D':
        case 'd':
          nDoubleWordChecksum = 1;
          break ;
        case 'H':
        case 'h':
        case '?':
          ShowUsage();
          exit(0);
          break ;
        default :
          break ;
        }
      }
    }
  }
  //
  // _open Target file
  //
  if (_sopen_s (&fhSrc, argv[1], _O_BINARY | _O_RDONLY, _SH_DENYWR, _S_IREAD) != 0)
  //if ((fhSrc=_open(argv[1],O_BINARY))==-1)
  {
    printf("%s _open error!\n",argv[1]);
    exit(-2);
  }
  //
  // Checksum by byte thru the file
  //
  _lseek( fhSrc, 0L, SEEK_SET);           /* Set to position 0 */
  Checksum=0;
  lCurrentOffset=0;
  while ( _read( fhSrc, bBuffer, SIZE_BUFFER))
  {
    //
    // Check if exceed size requested?
    //
    if( lROMSize && lCurrentOffset >= lROMSize)
      break;
    //
    // Accumulate _read buffer
    //    
    for (i=0; i<SIZE_BUFFER; i++)
    {
      //
      // Skip BIOSInfo block if requested & exist
      //
      Checksum += (DWORD)bBuffer[i];
    }
    lCurrentOffset += SIZE_BUFFER;
  }
  //
  // Set file size
  //
  lROMSize = lCurrentOffset;
  //
  // Show result by requested format
  //
  fprintf(stderr, "Byte Checksum V1.00\n");
  fprintf(stderr, "Copyleft PX 2014\n");
  printf("Checksum Size = %dMB\n", lROMSize>>20);
  if (nDoubleWordChecksum == 1) {
    printf("Checksum = %08lX\n", Checksum);
  } else {
    printf("Checksum = %04X\n", Checksum & 0xffff);
  }

  _close(fhSrc);
  exit(0);
}
