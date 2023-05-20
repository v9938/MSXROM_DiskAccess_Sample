# MSX ROM Disk Access Sample code
MSXのROM CartridgeからDisk読み込みをするサンプルコードです。  
スペースキーを押すと、ディスクのセクターReadを実行し、先頭128Byteを表示します。  
  
  
This is a sample code for reading from a disk on MSX ROM cartridge.  
When the space key is pressed, it executes a sector read from the disk and displays the first 128 bytes.  


## Notes for Compilation
コードのコンパイルは、z88dkを使用しています。`cc.bat`でコンパイルできます。  
コンパイル時は、ROMのスタートアップファイルの差し替えが必要です。  
z88dkインストールフォルダーの以下のファイルを、同梱してある同名ファイルと差し替えてください。  
  
`.\lib\target\msx\classic\rom.asm`
  
※元ファイルのバックアップはしてください。  
  
  
The code is compiled using z88dk.The code is compiled using z88dk. Compilation using `cc.bat`.  
During compilation, it is necessary to replace the startup file of the ROM.  
Please replace the following file in the z88dk installation folder with the corresponding file included in this package  

`.\lib\target\msx\classic\rom.asm`

Please dont forget a backup of the original file :-)  

