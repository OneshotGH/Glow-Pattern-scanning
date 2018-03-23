
                   /* @'   @+          @;         @@@@@         @@@@@,                  @                                
                      @@  ;@+          @;         @,  @         @'  @,                  @           @                    
                      @@, @@+ +@@@: @@@@; @@@@    @,  @ @; '@   @'  @, @@@@  @@@@ :@@@' @@@@, @@@@:@@@#                    
                      @,@.@#+ ,. ## @  @; @  @    @@@@@ ,@ @:   @'  @, @` @` @  @ +@ :; @, @; @  @' @.                    
                      @ @@:#+ '@@@# @  @; @@@@    @,  @  @ @    @'  @, @  @` @@@@  @@@  @  @; @  @' @.                     
                      @ '@ #+ @. @# @  @; @  .    @,  @  '@     @;  @, @  @` @  . `` +@ @  @; @  @' @.                      
                      @    #+ @@@@# @@@@; @@@@    @@@@@ :@:     @@@@@. @  @. @@@@ '@@@@ @` @; @@@@' @@+ */             
#include "OneshotsGlowHack.h"
CHackProcess fProcess; // Object made so we can use the class functions
Glow G; // GlowClass Object 
int main() // our start this code runs first.
{
	std::cout << "Oneshots GlowHack. F1 to toggle Friendemys." << std::endl;
	fProcess.RunProcess(); // runing RunProcess() that setups everything we need to read and write memory.

	O.dwGlowObject = (DWORD)PatternScanExModule(fProcess.__HandleProcess, L"csgo.exe", L"client.dll",
		"\x0F\x11\x05\x00\x00\x00\x00\x83\xC8\x01", "xxx????xxx");
	O.local = (DWORD)PatternScanExModule(fProcess.__HandleProcess, L"csgo.exe", L"client.dll",
		"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x8B\x07\x8B\xCF\x8B\x75\x08", "xx????xxxx????xxxxxxx");
	
	if (!O.local || !O.dwGlowObject)
	{
		std::cout << "pattern failed" << std::endl;
		Beep(500, 1000);
		return 0;
	}
	std::cout << "Game Found Starting Glow" << std::endl;
	while (true) // a while loop that we have to run our code over and over until we shut it down.
	{
		G.Run();
		Sleep(5); // Sleep of 1ms so we dont eat to much cpu power.
	}
	
    return 0; 
}

