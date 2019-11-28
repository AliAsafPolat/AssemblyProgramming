#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void sagaDondur(short n, int resim);
void solaDondur(short n, int resim);

int main(void) {
	int M, N, Q, i, j, k;
	bool type;
	int efile, islem;
	char resimadi[100], sonek[10];
	do {
		printf("Islem yapilacak resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &resimadi);
		system("CLS");
		efile = readImageHeader(resimadi, N, M, Q, type);
	} while (efile > 1);
	printf("%s\n", resimadi);
	int** resim = resimOku(resimadi);

	short *resimdizi;
	resimdizi = (short*)malloc(N*M * sizeof(short));

	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++)
			resimdizi[i*N + j] = (short)resim[i][j];

	int resimadres = (int)resimdizi;

	do {
		system("CLS");
		printf("\t     ISLEMLER\n");
		printf("------------------------------------\n");
		printf("1)  Resmi saga dondur\n");
		printf("2)  Resmi sola dondur\n");
		printf("0)  Cikis\n\n");
		printf("\"%s\" yolundaki resim icin yapilacak islemi seciniz\n-> ", resimadi);
		scanf("%d", &islem);
	} while (islem > 2 || islem < 0);

	switch (islem) {
	case 0:
		exit(0);
	case 1:
		sagaDondur(N, resimadres);
		strcpy(sonek, "_sag.pgm");
		break;
	case 2:
		solaDondur(N, resimadres);
		strcpy(sonek, "_sol.pgm");
		break;
	default:
		strcpy(sonek, "_orj.pgm");
		break;
	}

	for (k = 0; k < N * M; k++) {
		j = k % N;
		i = k / N;
		resim[i][j] = (int)resimdizi[k];
	}

	string::size_type pos = string(resimadi).find_last_of(".");
	resimadi[pos] = '\0';
	strcat(resimadi, sonek);
	resimYaz(resimadi, resim, N, M, Q);
	printf("\nIslem basariyla tamamlandi :)\n\"%s\" yolunda resim olusturuldu.\n\n", resimadi);
	system("PAUSE");
	return 0;
}

void sagaDondur(short N, int resim) {
	//KODUNUZU BURADAN BASLAYARAK YAZINIZ
	__asm {
		//Ýlk iþim asal köþegene göre transpozesini almak.
		//Sonrasýnda saða veya sola çevirme durumlarýna göre islem uygulamak.
		//Bu kýsýmda saga cevirme anlatýlacaktýr.
		//Saga cevirme: Transpozesini al.Sonrasýnda sütun sayýsýnýn yarýsýndan itibaren simetri al.

		XOR ESI, ESI		//ESI :Dýþ döngünün i deðeri.
		XOR EBX, EBX		//EBX :Ýç  döngünün j deðeri.
		XOR EAX, EAX		//EAX :Ýþlemlerimi yaptýðým register.
		XOR ECX, ECX		//ECX :Döngü deðerlerimi tuttuðum register.
		XOR EDI, EDI		//EDI :Resimi atadýðým register .


		MOV CX, N			//Döngü deðerimi atadým.

		Lopp1 :

			XOR EAX, EAX		//AX registerý akümülator olarak kullanýlacak.Sýfýrladým.
		PUSH CX					//Ýçteki for için CX kullanacagým.Dýþ döngümü kaybetmemek icin yýðýna atama yaptým.
			MOV EAX, ESI		//Dýþ döngümün deðeri N-i kadar olacak.
			SHR EAX, 1			//SI yi her turda 2 arttýrdýðým için ikiye böldüm.Amacým i deðerini elde etmek. 
			MOV CX, N			//CX registerýna N deðeri atandý.
			SUB ECX, EAX		//Þimdi N-i deðeri gerçeklenmiþ oldu.Ýç döngümün deðeri artýk belli.

			XOR EBX, EBX		//Ýç döngümün deðeri her turda i ye eþit olacak.
								//Transpoze alan algoritmada j=i den baþlýyorduk hatýrlarsak.
			MOV EBX, ESI		//j=i gerçeklendi.

			Lopp2 :

			XOR EAX, EAX		//EAX registerýný her bir dönme için kullanacaðým.Bu yüzden içinde deðer býrakmýyorum.		
			XOR EDX, EDX		//EDX registerýný her bir döngü için kullanacaðým.Ýçinde deðer býrakmýyorum. 

			MOV AX, N			//N*ESI+EBX BULUNDUGUM YERI GOSTERIYOR.Matris olarak düþünürsek N*i+j bulundugum yerdir.	
			MUL ESI				//N*ESI gerçeklendi.
			ADD EAX, EBX		//N*ESI + EBX gerçeklendi.
		
		PUSH CX					//EAX üzerinden iþlem yapmaya devam edeceðim. 					
			MOV ECX,EAX			//Bu sebeple ECX registerýna EAX i alýp daha sonra tekrardan EAX e atayacaðým.
			XOR EAX,EAX			//EAX deðerini kullanacaðýmdan içinde deðer býrakmýyorum.
		PUSH BX					//EBX registerý hatýrladýðýmýz gibi iç döngü elemanýydý.Ben EBX registarýný   
								//yer deðiþtireceðim deðiþkenlerde hedef adresi gösteren olarak ayarlayacaðým. 	
								//Swap iþlemi bittikten sonra iç döngü deðerimin deðiþmesini istemediðim için yýðýna atýyorum.	
			

			MOV AX, N			//N*EBX+ESI GITMEK ISTEDIGIM YERI GOSTERIYOR.	
			MUL EBX				//Mantýken arr[i][j]=arr[j][i] yapacaðým.Dolayýsýyla gitmek istediðim adres N*EBX+ESI olmalý.
			ADD EAX, ESI		//N*EBX+ESI gerçeklendi.	
			MOV EBX,EAX			//Hedef adresim EBX registera atandý.

			MOV EAX,ECX			//BULUNDUGUM YER ECX DEN GERI ALINDI.Daha önce saklamak için ECX'e almýþtým hatýrlarsak.



								//**SWAP ÝÞLEMLERÝ BAÞLIYOR.**
								//EAX:Bulunduðum yeri iþaret ediyor.
								//EBX:Bulunduðum yer ile swap yapmak istediðim kýsmý iþaret ediyor.

			
				
			XOR EDX, EDX		//DX Ý TMP OLARAK KULLANACAGIM.

			MOV EDI, resim		//EDI registerina resimi her atadýðýmda en baþýný gösteriyor.Ben sadece baþýndan istediðim kadar ilerleyip
								//swap iþlemini gerçeklemek istiyorum.Yani EDI yý sadece resime ulaþmak için kullanýyorum.

			ADD EDI, EBX		//Resimdeki hedef adresime ulaþýyorum.
			MOV DX, WORD PTR[EDI]//Hedef adresdeki deðeri DX:tmp olarak alýyorum.
		PUSH DX					//Hedefteki deðeri saklýyorum daha sonra bulunduðum yere atayacaðým.
			XOR DX, DX			//DX registerý kullanacaðým.Ýçinde deðer býrakmýyorum.
			
				
			XOR EDI, EDI		
			MOV EDI, resim		//Resim EDI ya alýndý.
			ADD EDI, EAX		//Bulunduðum yer EAX de saklanýyordu.Baþlangýçtan EAX kadar ilerledim 
			MOV DX, WORD PTR[EDI]//Bulunduðum yerdeki deðeri tmp olarak aldým.DÝKKAT!:Daha önceden hedefi alýp yýðýna atmýþtým.

			XOR EDI, EDI		
			MOV EDI, resim		//Resmin en baþýna gidiyorum.
			ADD EDI, EBX		//Hedef kadar ilerliyorum.
			MOV WORD PTR[EDI], DX//Önceki adýmda bulunduðum yerden aldýðým DX deðerini atýyorum.Swapýn ilk adýmý gerçekleþti.

		POP DX					//Hedeften alýp yýðýna attýðým Tmp deðerini çekiyorum.

			XOR EDI, EDI
			MOV EDI, resim		//Resmin baþlangýcýndan bulunduðum yer kadar ilerliyorum
			ADD EDI, EAX		
			MOV WORD PTR[EDI], DX//Yýðýndan çektiðim hedef deðerini bulunduðum yere koyup swapý tamamlýyorum.		
			
			XOR ECX,ECX			//CX deðerini sýfýrlýyorum. 
			XOR EBX,EBX			//EBX deðerini sýfýrlýyorum.
		POP BX					//Yýðýna attýðým iç döngü deðiþkenini çekiyorum böylelikle ara iþlemlerde döngü deðerim korunmuþ oluyor.
		POP CX					//CX i EAX deðerini saklanmak için yýðýna atmýþtým.Çekiyorum. 
			ADD EBX, 2			//Ýç döngü deðerimi 2 arttýrýyorum.
			LOOP Lopp2	

			POP CX				//Dýþ döngü deðerini çekiyorum.
			ADD ESI, 2			//Dýþ döngü indisimi 2 artýrýyorum.

			LOOP Lopp1

					
				
								//TRANSPOZE TAMAM.
								//Bu kýsýmda saða çevirmek istediðimden matrisin ortasýndaki sütünuna göre simetri alacaðým.

			//Kullanacaðým registerlarý önceki deðerleri (kalmýþ ise) sýfýrlýyorum.
			
			XOR EAX,EAX		//EAX :Bulundugum yer olacak.			
			XOR EBX,EBX		//EBX :Ýç döngü deðerim olacak.Ýlerleyen kýsýmda ise hedef deðerim olacak. 
			XOR ECX,ECX		//ECX :Döngü deðerim olacak.
			XOR EDX, EDX	//EDX :Tmp olarak kullanacaðým.	
			XOR EDI,EDI		//EDI :Resimin baþlangýç adresini tutacak.
			XOR ESI,ESI		//ESI :Dýþ döngü deðerim olacak.

			MOV CX,N		//Dýþ döngü deðeri atandý.

	Lup1:	
			PUSH CX			//Dýþ döngü deðerini kaybetmemek için yýðýna atýldý.
			MOV CX,N		//Matrisin ortadaki sütununa göre simetri alacaktým 
			SHR CX,1		//N deðerinin yarýsý alýnarak gerçeklendi.
			XOR EBX,EBX		//Ýç döngü her bittiðinde EBX deðeri(indis) sýfýrlanýyor. 
	Lup2:	
			XOR EAX,EAX		//EAX' bulunduðum yeri atacaðým yani EAX=N*ESI+EBX(N*i+j)	
			MOV AX,N		
			MUL ESI			//ESI*N gerçeklendi.
			ADD EAX,EBX		//ESI*N+EBX gerçeklendi.**BURASI BULUNDUGUM YER**
		
		PUSH CX				//EAX registerý iþlemler için kullanýlacaðýndan ECX'i EAX deðerini saklamak için kullanacaðým.
							//Döngü deðerimi kaybetmemek için yýðýna atýyorum.
			
			MOV ECX,EAX		//EAX üzerinden iþlem yapacaðým için ECX deðerinde saklýyorum.
		
			XOR EAX,EAX		//Artýk EAX registerýný sýfýrlayabilirim.			
			
			MOV AX,N		//Önce EBX deðeri belirlenecek.Sütunlar arasý yer deðiþimi olacak yani arr[i][j]=arr[i][n-j-1] 
							//Buradaki n-j-1 deðeri baktýðýmýz zaman iç döngü deðerine karþýlýk geliyor o halde EBX i güncellemem gerekli.
			SHL EAX, 1		//Her elemaným 2 byte olduðundan N deðerini 2 ile çarpýyorum aslýnda.
			SUB EAX,2		//Haliyle -1 deðerim de -2 olacak. 
			SUB EAX,EBX		//Þimdi ise -j deðerini gerçekledim.
		PUSH BX				//Bu iþlemleri yaparken iç döngü deðerimi kaybetmek istemiyorum.Bunu yýðýna atýyorum.
			XOR EBX,EBX
			MOV EBX,EAX		//Artýk [n-j-1] deðerim belli ve bunu EBX atýyorum.Hedef deðerini hesaplarken bu indis kullanýlacak.
			
			XOR EAX,EAX		//EAX kullanýlacak.Sýfýrlýyorum.

			MOV AX,N		//Hedef deðerimi belirliyorum.
			MUL ESI			//Dýþ döngü deðerimle çarpýyorum.[i].
			ADD EAX,EBX		//[n-j-1] deðerimle topluyorum.
			MOV EBX,EAX		//**ARTIK HEDEF ADRESÝM BELLI**.
			
			XOR EAX,EAX
			
			MOV EAX,ECX		//Bulunduðum yeri ECX'e atmýþtým o deðeri geri çekiyorum.
			XOR EDX, EDX	//DX Tmp olarak olarak kullanýlacak.

			MOV EDI, resim	//Resimin baþlangýcý alýndý.
			ADD EDI, EBX	//Hedef adresime ulaþtým.	
			MOV DX, WORD PTR[EDI]//Hedefteki deðeri Tmp olarak aldým.
		PUSH DX				//Ve yýðýna attým.
			XOR DX, DX		//Tekrar tmp olarak kullanýlcak.Sýfýrlandý.
			XOR EDI, EDI	
			MOV EDI, resim	//Resimin baþlangýç adresi alýndý.
			ADD EDI, EAX	//Bulunduðum yer kadar ilerledim.	
			MOV DX, WORD PTR[EDI]//Bulunduðum yerdeki deðer Tmp olarak alýndý.

			XOR EDI, EDI
			MOV EDI, resim	//Resimin baþlangýç adresi alýndý.
			ADD EDI, EBX	//Hedef deðerime gidiyorum.
			MOV WORD PTR[EDI], DX//Bulundugum yerden aldýgým degeri hedef deðere atýyorum ve Swapýn ilk adýmý gerçekleþiyor.

		POP DX				//Yýðýna attýðým hedefdeki deðer çekiliyor.

			XOR EDI, EDI
			MOV EDI, resim	//Resmin en baþýna gidiyorum
			ADD EDI, EAX	//Bulundugum yer kadar ilerliyorum	
			MOV WORD PTR[EDI], DX//Bulundugum yere hedefteki deðer atýlýp Swap tamamlanýyor.	

			XOR ECX, ECX
			XOR EBX, EBX
		POP BX				//Ýç döngü indisini korumak için yýðýna atmýþtým.Çekiyorum.
		POP CX				//ECX, EAX deðerini korumak için kullanýlmýþtý.Ýç döngü deðerimi çekiyorum.
			ADD EBX, 2		//Ýç döngü indisini artýrýyorum.

			LOOP Lup2

			POP CX			//Dýþ döngü deðerini çekiyorum.
			ADD ESI, 2		//Dýþ döngü indisini artýrýyorum.	

			LOOP Lup1







	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ
}

void solaDondur(short N, int resim) {
	//KODUNUZU BURADAN BASLAYARAK YAZINIZ
	__asm {
		//Ýlk iþim asal köþegene göre transpozesini almak.
		//Sonrasýnda saða veya sola çevirme durumlarýna göre islem uygulamak.
		//Bu kýsýmda sola cevirme anlatýlacaktýr.Transpoze alana kadar saða çevirmeyle ayný iþlemler uygulandý.
		//Sola cevirme: Transpozesini al.Sonrasýnda satýr sayýsýnýn yarýsýndan itibaren simetri al.

		XOR ESI, ESI		//ESI :Dýþ döngünün i deðeri.
		XOR EBX, EBX		//EBX :Ýç  döngünün j deðeri.
		XOR EAX, EAX		//EAX :Ýþlemlerimi yaptýðým register.
		XOR ECX, ECX		//ECX :Döngü deðerlerimi tuttuðum register.
		XOR EDI, EDI		//EDI :Resimi atadýðým register .


		MOV CX, N			//Döngü deðerimi atadým.

		Lupp1 :

		XOR EAX, EAX		//AX registerý akümülator olarak kullanýlacak.Sýfýrladým.
			PUSH CX					//Ýçteki for için CX kullanacagým.Dýþ döngümü kaybetmemek icin yýðýna atama yaptým.
			MOV EAX, ESI		//Dýþ döngümün deðeri N-i kadar olacak.
			SHR EAX, 1			//SI yi her turda 2 arttýrdýðým için ikiye böldüm.Amacým i deðerini elde etmek. 
			MOV CX, N			//CX registerýna N deðeri atandý.
			SUB ECX, EAX		//Þimdi N-i deðeri gerçeklenmiþ oldu.Ýç döngümün deðeri artýk belli.

			XOR EBX, EBX		//Ýç döngümün deðeri her turda i ye eþit olacak.
								//Transpoze alan algoritmada j=i den baþlýyorduk hatýrlarsak.
			MOV EBX, ESI		//j=i gerçeklendi.

			Lupp2 :

		XOR EAX, EAX		//EAX registerýný her bir dönme için kullanacaðým.Bu yüzden içinde deðer býrakmýyorum.		
			XOR EDX, EDX		//EDX registerýný her bir döngü için kullanacaðým.Ýçinde deðer býrakmýyorum. 

			MOV AX, N			//N*ESI+EBX BULUNDUGUM YERI GOSTERIYOR.Matris olarak düþünürsek N*i+j bulundugum yerdir.	
			MUL ESI				//N*ESI gerçeklendi.
			ADD EAX, EBX		//N*ESI + EBX gerçeklendi.

			PUSH CX					//EAX üzerinden iþlem yapmaya devam edeceðim. 					
			MOV ECX, EAX			//Bu sebeple ECX registerýna EAX i alýp daha sonra tekrardan EAX e atayacaðým.
			XOR EAX, EAX			//EAX deðerini kullanacaðýmdan içinde deðer býrakmýyorum.
			PUSH BX					//EBX registerý hatýrladýðýmýz gibi iç döngü elemanýydý.Ben EBX registarýný   
									//yer deðiþtireceðim deðiþkenlerde hedef adresi gösteren olarak ayarlayacaðým. 	
									//Swap iþlemi bittikten sonra iç döngü deðerimin deðiþmesini istemediðim için yýðýna atýyorum.	


			MOV AX, N			//N*EBX+ESI GITMEK ISTEDIGIM YERI GOSTERIYOR.	
			MUL EBX				//Mantýken arr[i][j]=arr[j][i] yapacaðým.Dolayýsýyla gitmek istediðim adres N*EBX+ESI olmalý.
			ADD EAX, ESI		//N*EBX+ESI gerçeklendi.	
			MOV EBX, EAX			//Hedef adresim EBX registera atandý.

			MOV EAX, ECX			//BULUNDUGUM YER ECX DEN GERI ALINDI.Daha önce saklamak için ECX'e almýþtým hatýrlarsak.



								//**SWAP ÝÞLEMLERÝ BAÞLIYOR.**
								//EAX:Bulunduðum yeri iþaret ediyor.
								//EBX:Bulunduðum yer ile swap yapmak istediðim kýsmý iþaret ediyor.



			XOR EDX, EDX		//DX Ý TMP OLARAK KULLANACAGIM.

			MOV EDI, resim		//EDI registerina resimi her atadýðýmda en baþýný gösteriyor.Ben sadece baþýndan istediðim kadar ilerleyip
								//swap iþlemini gerçeklemek istiyorum.Yani EDI yý sadece resime ulaþmak için kullanýyorum.

			ADD EDI, EBX		//Resimdeki hedef adresime ulaþýyorum.
			MOV DX, WORD PTR[EDI]//Hedef adresdeki deðeri DX:tmp olarak alýyorum.
			PUSH DX					//Hedefteki deðeri saklýyorum daha sonra bulunduðum yere atayacaðým.
			XOR DX, DX			//DX registerý kullanacaðým.Ýçinde deðer býrakmýyorum.


			XOR EDI, EDI
			MOV EDI, resim		//Resim EDI ya alýndý.
			ADD EDI, EAX		//Bulunduðum yer EAX de saklanýyordu.Baþlangýçtan EAX kadar ilerledim 
			MOV DX, WORD PTR[EDI]//Bulunduðum yerdeki deðeri tmp olarak aldým.DÝKKAT!:Daha önceden hedefi alýp yýðýna atmýþtým.

			XOR EDI, EDI
			MOV EDI, resim		//Resmin en baþýna gidiyorum.
			ADD EDI, EBX		//Hedef kadar ilerliyorum.
			MOV WORD PTR[EDI], DX//Önceki adýmda bulunduðum yerden aldýðým DX deðerini atýyorum.Swapýn ilk adýmý gerçekleþti.

			POP DX					//Hedeften alýp yýðýna attýðým Tmp deðerini çekiyorum.

			XOR EDI, EDI
			MOV EDI, resim		//Resmin baþlangýcýndan bulunduðum yer kadar ilerliyorum
			ADD EDI, EAX
			MOV WORD PTR[EDI], DX//Yýðýndan çektiðim hedef deðerini bulunduðum yere koyup swapý tamamlýyorum.		

			XOR ECX, ECX			//CX deðerini sýfýrlýyorum. 
			XOR EBX, EBX			//EBX deðerini sýfýrlýyorum.
			POP BX					//Yýðýna attýðým iç döngü deðiþkenini çekiyorum böylelikle ara iþlemlerde döngü deðerim korunmuþ oluyor.
			POP CX					//CX i EAX deðerini saklanmak için yýðýna atmýþtým.Çekiyorum. 
			ADD EBX, 2			//Ýç döngü deðerimi 2 arttýrýyorum.
			LOOP Lupp2

			POP CX				//Dýþ döngü deðerini çekiyorum.
			ADD ESI, 2			//Dýþ döngü indisimi 2 artýrýyorum.

			LOOP Lupp1

			//TRANSPOZE TAMAM.
								//Bu kýsýmda sola çevirmek istediðimden matrisin ortasýndaki satýra göre simetri alacaðým.

			//Kullanacaðým registerlarý önceki deðerleri (kalmýþ ise) sýfýrlýyorum.

			XOR EAX, EAX		//EAX :Bulundugum yer olacak.			
			XOR EBX, EBX		//EBX :Ýç döngü deðerim olacak.Ýlerleyen kýsýmda ise hedef deðerim olacak. 
			XOR ECX, ECX		//ECX :Döngü deðerim olacak.
			XOR EDX, EDX		//EDX :Tmp olarak kullanacaðým.	
			XOR EDI, EDI		//EDI :Resimin baþlangýç adresini tutacak.
			XOR ESI, ESI		//ESI :Dýþ döngü deðerim olacak.

			MOV CX, N			//Bu kýsýmda satýr sayýsýnýn yarýsýna kadar gideceðim.
			SHR CX,1			//Çünkü satýrlarýn ortasýna göre simetri alýyorum.


			Lup1 :
		PUSH CX					//Dýþ döngü deðerimi kaybetmek istemiyorum.Yýðýna atýldý.
			MOV CX, N			//Ýç döngü deðerim atandý.Tüm sütunlar gezilecek N atandý.
			XOR EBX, EBX		//Ýç döngü deðerim dýþ döngü her döndüðünde sýfýrlanacak. 
			Lup2 :
			XOR EAX, EAX		//Bulunduðum yeri belirleyeceðim.
			MOV AX, N			//N*ESI+EBX (N*i+j) bulunduðum yerdir.
			MUL ESI				//N*ESI gerçeklendi
			ADD EAX, EBX		//N*ESI+EBX gerçeklendi.**BURASI BULUNDUGUM YER**

		PUSH CX					//EAX'deki deðeri saklamam gerekecek.Döngü deðerimi saklýyorum.

			MOV ECX, EAX		//EAX iþlem yapacaðým için buradaki deðerimi ECX de sakladým.Ýþlemler bittikten sonra tekrar EAX'e atýlacak.

			XOR EAX, EAX

			MOV AX, N		//Satýr üzerinde iþlem yapacaðým yani arr[i][j]=arr[n-i-1][j] olacak.
			SHL EAX, 1		//Burada word tipinde çalýþtýðým için aslýnda 2*N yapmalýyým.
			SUB EAX, 2		//-1 ise -2 deðerinde olacak.(word)
			SUB EAX, ESI	//-i deðerini gerçekledim
		PUSH BX				//EBX hedef deðerimi gösterecek.Döngüdeki indis deðerini kaybetmemesi için yýðýna atýyorum.
		PUSH SI				//ESI deðiþen indis deðerini yani(n-i-1) i gösterecek.Dýþ döngü deðerini kaybetmek istemiyorum.
			
			XOR ESI, ESI
			MOV ESI, EAX		//ESI = (n-i-1) gerçeklendi.
			XOR EAX, EAX

			MOV AX, N			//Hedef deðerimi hesaplayacaðým.
			MUL ESI				//Satýr sayýsýyla boyutu çarpýyorum.DÝKKAT!:Deðiþen satýr sayýsýyla iþlem yapýldý.[i][j] deðil [n-i-1][j] yani.
			ADD EAX, EBX		//N*ESI+EBX gerçeklendi.
			MOV EBX, EAX		//EBX hedef deðerim.***ARTIK HEDEF DEGERÝ BELLI**
		POP SI					//SI ile iþim bitti dýþ döngü deðerimi korumuþ oldum.
			XOR EAX, EAX		

			MOV EAX, ECX		//Bulunduðum yeri ECX de korumuþtum.Çekiyorum.
			XOR EDX, EDX		//DX Tmp olarak kullanýlacak.

			MOV EDI, resim		//Resmin baþlangýcý alýndý.
			ADD EDI, EBX		//Hedef adresine gidildi.
			MOV DX, WORD PTR[EDI]//Hedefteki deðer Tmp olarak alýndý..
		PUSH DX					//Hedefteki deðeri yýðýnda saklýyorum.
			XOR DX, DX
			XOR EDI, EDI
			MOV EDI, resim		//Resmin baþlangýç adresine gidildi.
			ADD EDI, EAX		//Bulundugum yere ilerledim.
			MOV DX, WORD PTR[EDI]//Bulunduðum yerdeki deðer Tmp olarak alýndý.

			XOR EDI, EDI
			MOV EDI, resim		//Resmin baþlangýç adresine gidildi.
			ADD EDI, EBX		//Hedef konuma ilerledim
			MOV WORD PTR[EDI], DX//Bulunduðum yerden aldýðým deðeri hedef deðerine yazdým swapýn ilk adýmý gerçekleþti.

		POP DX				//Hedeften aldýðým tmp deðeri çekiyorum

			XOR EDI, EDI
			MOV EDI, resim		//Resmin baþlangýç adresi alýndý.
			ADD EDI, EAX		//Bulundugum adrese gidildi.
			MOV WORD PTR[EDI], DX//Hedeften aldýðým deðer atýldý.

			XOR ECX, ECX
			XOR EBX, EBX
		POP BX					//Ýç döngü indisim bozulmadan yýðýndan alýndý.
		POP CX					//ECX de EAX korunmuþtu. Döngü deðerimi geri çekiyorum.
			ADD EBX, 2			//Ýç döngü indisini artýrýyorum

			LOOP Lup2

			POP CX				//Dýþ döngü deðerim çekiliyor.
			ADD ESI, 2			//Dýþ döngü indisim artýrýldý.

			LOOP Lup1

	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ

}