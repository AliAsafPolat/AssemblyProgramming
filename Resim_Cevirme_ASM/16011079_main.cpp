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
		//�lk i�im asal k��egene g�re transpozesini almak.
		//Sonras�nda sa�a veya sola �evirme durumlar�na g�re islem uygulamak.
		//Bu k�s�mda saga cevirme anlat�lacakt�r.
		//Saga cevirme: Transpozesini al.Sonras�nda s�tun say�s�n�n yar�s�ndan itibaren simetri al.

		XOR ESI, ESI		//ESI :D�� d�ng�n�n i de�eri.
		XOR EBX, EBX		//EBX :��  d�ng�n�n j de�eri.
		XOR EAX, EAX		//EAX :��lemlerimi yapt���m register.
		XOR ECX, ECX		//ECX :D�ng� de�erlerimi tuttu�um register.
		XOR EDI, EDI		//EDI :Resimi atad���m register .


		MOV CX, N			//D�ng� de�erimi atad�m.

		Lopp1 :

			XOR EAX, EAX		//AX register� ak�m�lator olarak kullan�lacak.S�f�rlad�m.
		PUSH CX					//��teki for i�in CX kullanacag�m.D�� d�ng�m� kaybetmemek icin y���na atama yapt�m.
			MOV EAX, ESI		//D�� d�ng�m�n de�eri N-i kadar olacak.
			SHR EAX, 1			//SI yi her turda 2 artt�rd���m i�in ikiye b�ld�m.Amac�m i de�erini elde etmek. 
			MOV CX, N			//CX register�na N de�eri atand�.
			SUB ECX, EAX		//�imdi N-i de�eri ger�eklenmi� oldu.�� d�ng�m�n de�eri art�k belli.

			XOR EBX, EBX		//�� d�ng�m�n de�eri her turda i ye e�it olacak.
								//Transpoze alan algoritmada j=i den ba�l�yorduk hat�rlarsak.
			MOV EBX, ESI		//j=i ger�eklendi.

			Lopp2 :

			XOR EAX, EAX		//EAX register�n� her bir d�nme i�in kullanaca��m.Bu y�zden i�inde de�er b�rakm�yorum.		
			XOR EDX, EDX		//EDX register�n� her bir d�ng� i�in kullanaca��m.��inde de�er b�rakm�yorum. 

			MOV AX, N			//N*ESI+EBX BULUNDUGUM YERI GOSTERIYOR.Matris olarak d���n�rsek N*i+j bulundugum yerdir.	
			MUL ESI				//N*ESI ger�eklendi.
			ADD EAX, EBX		//N*ESI + EBX ger�eklendi.
		
		PUSH CX					//EAX �zerinden i�lem yapmaya devam edece�im. 					
			MOV ECX,EAX			//Bu sebeple ECX register�na EAX i al�p daha sonra tekrardan EAX e atayaca��m.
			XOR EAX,EAX			//EAX de�erini kullanaca��mdan i�inde de�er b�rakm�yorum.
		PUSH BX					//EBX register� hat�rlad���m�z gibi i� d�ng� eleman�yd�.Ben EBX registar�n�   
								//yer de�i�tirece�im de�i�kenlerde hedef adresi g�steren olarak ayarlayaca��m. 	
								//Swap i�lemi bittikten sonra i� d�ng� de�erimin de�i�mesini istemedi�im i�in y���na at�yorum.	
			

			MOV AX, N			//N*EBX+ESI GITMEK ISTEDIGIM YERI GOSTERIYOR.	
			MUL EBX				//Mant�ken arr[i][j]=arr[j][i] yapaca��m.Dolay�s�yla gitmek istedi�im adres N*EBX+ESI olmal�.
			ADD EAX, ESI		//N*EBX+ESI ger�eklendi.	
			MOV EBX,EAX			//Hedef adresim EBX registera atand�.

			MOV EAX,ECX			//BULUNDUGUM YER ECX DEN GERI ALINDI.Daha �nce saklamak i�in ECX'e alm��t�m hat�rlarsak.



								//**SWAP ��LEMLER� BA�LIYOR.**
								//EAX:Bulundu�um yeri i�aret ediyor.
								//EBX:Bulundu�um yer ile swap yapmak istedi�im k�sm� i�aret ediyor.

			
				
			XOR EDX, EDX		//DX � TMP OLARAK KULLANACAGIM.

			MOV EDI, resim		//EDI registerina resimi her atad���mda en ba��n� g�steriyor.Ben sadece ba��ndan istedi�im kadar ilerleyip
								//swap i�lemini ger�eklemek istiyorum.Yani EDI y� sadece resime ula�mak i�in kullan�yorum.

			ADD EDI, EBX		//Resimdeki hedef adresime ula��yorum.
			MOV DX, WORD PTR[EDI]//Hedef adresdeki de�eri DX:tmp olarak al�yorum.
		PUSH DX					//Hedefteki de�eri sakl�yorum daha sonra bulundu�um yere atayaca��m.
			XOR DX, DX			//DX register� kullanaca��m.��inde de�er b�rakm�yorum.
			
				
			XOR EDI, EDI		
			MOV EDI, resim		//Resim EDI ya al�nd�.
			ADD EDI, EAX		//Bulundu�um yer EAX de saklan�yordu.Ba�lang��tan EAX kadar ilerledim 
			MOV DX, WORD PTR[EDI]//Bulundu�um yerdeki de�eri tmp olarak ald�m.D�KKAT!:Daha �nceden hedefi al�p y���na atm��t�m.

			XOR EDI, EDI		
			MOV EDI, resim		//Resmin en ba��na gidiyorum.
			ADD EDI, EBX		//Hedef kadar ilerliyorum.
			MOV WORD PTR[EDI], DX//�nceki ad�mda bulundu�um yerden ald���m DX de�erini at�yorum.Swap�n ilk ad�m� ger�ekle�ti.

		POP DX					//Hedeften al�p y���na att���m Tmp de�erini �ekiyorum.

			XOR EDI, EDI
			MOV EDI, resim		//Resmin ba�lang�c�ndan bulundu�um yer kadar ilerliyorum
			ADD EDI, EAX		
			MOV WORD PTR[EDI], DX//Y���ndan �ekti�im hedef de�erini bulundu�um yere koyup swap� tamaml�yorum.		
			
			XOR ECX,ECX			//CX de�erini s�f�rl�yorum. 
			XOR EBX,EBX			//EBX de�erini s�f�rl�yorum.
		POP BX					//Y���na att���m i� d�ng� de�i�kenini �ekiyorum b�ylelikle ara i�lemlerde d�ng� de�erim korunmu� oluyor.
		POP CX					//CX i EAX de�erini saklanmak i�in y���na atm��t�m.�ekiyorum. 
			ADD EBX, 2			//�� d�ng� de�erimi 2 artt�r�yorum.
			LOOP Lopp2	

			POP CX				//D�� d�ng� de�erini �ekiyorum.
			ADD ESI, 2			//D�� d�ng� indisimi 2 art�r�yorum.

			LOOP Lopp1

					
				
								//TRANSPOZE TAMAM.
								//Bu k�s�mda sa�a �evirmek istedi�imden matrisin ortas�ndaki s�t�nuna g�re simetri alaca��m.

			//Kullanaca��m registerlar� �nceki de�erleri (kalm�� ise) s�f�rl�yorum.
			
			XOR EAX,EAX		//EAX :Bulundugum yer olacak.			
			XOR EBX,EBX		//EBX :�� d�ng� de�erim olacak.�lerleyen k�s�mda ise hedef de�erim olacak. 
			XOR ECX,ECX		//ECX :D�ng� de�erim olacak.
			XOR EDX, EDX	//EDX :Tmp olarak kullanaca��m.	
			XOR EDI,EDI		//EDI :Resimin ba�lang�� adresini tutacak.
			XOR ESI,ESI		//ESI :D�� d�ng� de�erim olacak.

			MOV CX,N		//D�� d�ng� de�eri atand�.

	Lup1:	
			PUSH CX			//D�� d�ng� de�erini kaybetmemek i�in y���na at�ld�.
			MOV CX,N		//Matrisin ortadaki s�tununa g�re simetri alacakt�m 
			SHR CX,1		//N de�erinin yar�s� al�narak ger�eklendi.
			XOR EBX,EBX		//�� d�ng� her bitti�inde EBX de�eri(indis) s�f�rlan�yor. 
	Lup2:	
			XOR EAX,EAX		//EAX' bulundu�um yeri ataca��m yani EAX=N*ESI+EBX(N*i+j)	
			MOV AX,N		
			MUL ESI			//ESI*N ger�eklendi.
			ADD EAX,EBX		//ESI*N+EBX ger�eklendi.**BURASI BULUNDUGUM YER**
		
		PUSH CX				//EAX register� i�lemler i�in kullan�laca��ndan ECX'i EAX de�erini saklamak i�in kullanaca��m.
							//D�ng� de�erimi kaybetmemek i�in y���na at�yorum.
			
			MOV ECX,EAX		//EAX �zerinden i�lem yapaca��m i�in ECX de�erinde sakl�yorum.
		
			XOR EAX,EAX		//Art�k EAX register�n� s�f�rlayabilirim.			
			
			MOV AX,N		//�nce EBX de�eri belirlenecek.S�tunlar aras� yer de�i�imi olacak yani arr[i][j]=arr[i][n-j-1] 
							//Buradaki n-j-1 de�eri bakt���m�z zaman i� d�ng� de�erine kar��l�k geliyor o halde EBX i g�ncellemem gerekli.
			SHL EAX, 1		//Her eleman�m 2 byte oldu�undan N de�erini 2 ile �arp�yorum asl�nda.
			SUB EAX,2		//Haliyle -1 de�erim de -2 olacak. 
			SUB EAX,EBX		//�imdi ise -j de�erini ger�ekledim.
		PUSH BX				//Bu i�lemleri yaparken i� d�ng� de�erimi kaybetmek istemiyorum.Bunu y���na at�yorum.
			XOR EBX,EBX
			MOV EBX,EAX		//Art�k [n-j-1] de�erim belli ve bunu EBX at�yorum.Hedef de�erini hesaplarken bu indis kullan�lacak.
			
			XOR EAX,EAX		//EAX kullan�lacak.S�f�rl�yorum.

			MOV AX,N		//Hedef de�erimi belirliyorum.
			MUL ESI			//D�� d�ng� de�erimle �arp�yorum.[i].
			ADD EAX,EBX		//[n-j-1] de�erimle topluyorum.
			MOV EBX,EAX		//**ARTIK HEDEF ADRES�M BELLI**.
			
			XOR EAX,EAX
			
			MOV EAX,ECX		//Bulundu�um yeri ECX'e atm��t�m o de�eri geri �ekiyorum.
			XOR EDX, EDX	//DX Tmp olarak olarak kullan�lacak.

			MOV EDI, resim	//Resimin ba�lang�c� al�nd�.
			ADD EDI, EBX	//Hedef adresime ula�t�m.	
			MOV DX, WORD PTR[EDI]//Hedefteki de�eri Tmp olarak ald�m.
		PUSH DX				//Ve y���na att�m.
			XOR DX, DX		//Tekrar tmp olarak kullan�lcak.S�f�rland�.
			XOR EDI, EDI	
			MOV EDI, resim	//Resimin ba�lang�� adresi al�nd�.
			ADD EDI, EAX	//Bulundu�um yer kadar ilerledim.	
			MOV DX, WORD PTR[EDI]//Bulundu�um yerdeki de�er Tmp olarak al�nd�.

			XOR EDI, EDI
			MOV EDI, resim	//Resimin ba�lang�� adresi al�nd�.
			ADD EDI, EBX	//Hedef de�erime gidiyorum.
			MOV WORD PTR[EDI], DX//Bulundugum yerden ald�g�m degeri hedef de�ere at�yorum ve Swap�n ilk ad�m� ger�ekle�iyor.

		POP DX				//Y���na att���m hedefdeki de�er �ekiliyor.

			XOR EDI, EDI
			MOV EDI, resim	//Resmin en ba��na gidiyorum
			ADD EDI, EAX	//Bulundugum yer kadar ilerliyorum	
			MOV WORD PTR[EDI], DX//Bulundugum yere hedefteki de�er at�l�p Swap tamamlan�yor.	

			XOR ECX, ECX
			XOR EBX, EBX
		POP BX				//�� d�ng� indisini korumak i�in y���na atm��t�m.�ekiyorum.
		POP CX				//ECX, EAX de�erini korumak i�in kullan�lm��t�.�� d�ng� de�erimi �ekiyorum.
			ADD EBX, 2		//�� d�ng� indisini art�r�yorum.

			LOOP Lup2

			POP CX			//D�� d�ng� de�erini �ekiyorum.
			ADD ESI, 2		//D�� d�ng� indisini art�r�yorum.	

			LOOP Lup1







	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ
}

void solaDondur(short N, int resim) {
	//KODUNUZU BURADAN BASLAYARAK YAZINIZ
	__asm {
		//�lk i�im asal k��egene g�re transpozesini almak.
		//Sonras�nda sa�a veya sola �evirme durumlar�na g�re islem uygulamak.
		//Bu k�s�mda sola cevirme anlat�lacakt�r.Transpoze alana kadar sa�a �evirmeyle ayn� i�lemler uyguland�.
		//Sola cevirme: Transpozesini al.Sonras�nda sat�r say�s�n�n yar�s�ndan itibaren simetri al.

		XOR ESI, ESI		//ESI :D�� d�ng�n�n i de�eri.
		XOR EBX, EBX		//EBX :��  d�ng�n�n j de�eri.
		XOR EAX, EAX		//EAX :��lemlerimi yapt���m register.
		XOR ECX, ECX		//ECX :D�ng� de�erlerimi tuttu�um register.
		XOR EDI, EDI		//EDI :Resimi atad���m register .


		MOV CX, N			//D�ng� de�erimi atad�m.

		Lupp1 :

		XOR EAX, EAX		//AX register� ak�m�lator olarak kullan�lacak.S�f�rlad�m.
			PUSH CX					//��teki for i�in CX kullanacag�m.D�� d�ng�m� kaybetmemek icin y���na atama yapt�m.
			MOV EAX, ESI		//D�� d�ng�m�n de�eri N-i kadar olacak.
			SHR EAX, 1			//SI yi her turda 2 artt�rd���m i�in ikiye b�ld�m.Amac�m i de�erini elde etmek. 
			MOV CX, N			//CX register�na N de�eri atand�.
			SUB ECX, EAX		//�imdi N-i de�eri ger�eklenmi� oldu.�� d�ng�m�n de�eri art�k belli.

			XOR EBX, EBX		//�� d�ng�m�n de�eri her turda i ye e�it olacak.
								//Transpoze alan algoritmada j=i den ba�l�yorduk hat�rlarsak.
			MOV EBX, ESI		//j=i ger�eklendi.

			Lupp2 :

		XOR EAX, EAX		//EAX register�n� her bir d�nme i�in kullanaca��m.Bu y�zden i�inde de�er b�rakm�yorum.		
			XOR EDX, EDX		//EDX register�n� her bir d�ng� i�in kullanaca��m.��inde de�er b�rakm�yorum. 

			MOV AX, N			//N*ESI+EBX BULUNDUGUM YERI GOSTERIYOR.Matris olarak d���n�rsek N*i+j bulundugum yerdir.	
			MUL ESI				//N*ESI ger�eklendi.
			ADD EAX, EBX		//N*ESI + EBX ger�eklendi.

			PUSH CX					//EAX �zerinden i�lem yapmaya devam edece�im. 					
			MOV ECX, EAX			//Bu sebeple ECX register�na EAX i al�p daha sonra tekrardan EAX e atayaca��m.
			XOR EAX, EAX			//EAX de�erini kullanaca��mdan i�inde de�er b�rakm�yorum.
			PUSH BX					//EBX register� hat�rlad���m�z gibi i� d�ng� eleman�yd�.Ben EBX registar�n�   
									//yer de�i�tirece�im de�i�kenlerde hedef adresi g�steren olarak ayarlayaca��m. 	
									//Swap i�lemi bittikten sonra i� d�ng� de�erimin de�i�mesini istemedi�im i�in y���na at�yorum.	


			MOV AX, N			//N*EBX+ESI GITMEK ISTEDIGIM YERI GOSTERIYOR.	
			MUL EBX				//Mant�ken arr[i][j]=arr[j][i] yapaca��m.Dolay�s�yla gitmek istedi�im adres N*EBX+ESI olmal�.
			ADD EAX, ESI		//N*EBX+ESI ger�eklendi.	
			MOV EBX, EAX			//Hedef adresim EBX registera atand�.

			MOV EAX, ECX			//BULUNDUGUM YER ECX DEN GERI ALINDI.Daha �nce saklamak i�in ECX'e alm��t�m hat�rlarsak.



								//**SWAP ��LEMLER� BA�LIYOR.**
								//EAX:Bulundu�um yeri i�aret ediyor.
								//EBX:Bulundu�um yer ile swap yapmak istedi�im k�sm� i�aret ediyor.



			XOR EDX, EDX		//DX � TMP OLARAK KULLANACAGIM.

			MOV EDI, resim		//EDI registerina resimi her atad���mda en ba��n� g�steriyor.Ben sadece ba��ndan istedi�im kadar ilerleyip
								//swap i�lemini ger�eklemek istiyorum.Yani EDI y� sadece resime ula�mak i�in kullan�yorum.

			ADD EDI, EBX		//Resimdeki hedef adresime ula��yorum.
			MOV DX, WORD PTR[EDI]//Hedef adresdeki de�eri DX:tmp olarak al�yorum.
			PUSH DX					//Hedefteki de�eri sakl�yorum daha sonra bulundu�um yere atayaca��m.
			XOR DX, DX			//DX register� kullanaca��m.��inde de�er b�rakm�yorum.


			XOR EDI, EDI
			MOV EDI, resim		//Resim EDI ya al�nd�.
			ADD EDI, EAX		//Bulundu�um yer EAX de saklan�yordu.Ba�lang��tan EAX kadar ilerledim 
			MOV DX, WORD PTR[EDI]//Bulundu�um yerdeki de�eri tmp olarak ald�m.D�KKAT!:Daha �nceden hedefi al�p y���na atm��t�m.

			XOR EDI, EDI
			MOV EDI, resim		//Resmin en ba��na gidiyorum.
			ADD EDI, EBX		//Hedef kadar ilerliyorum.
			MOV WORD PTR[EDI], DX//�nceki ad�mda bulundu�um yerden ald���m DX de�erini at�yorum.Swap�n ilk ad�m� ger�ekle�ti.

			POP DX					//Hedeften al�p y���na att���m Tmp de�erini �ekiyorum.

			XOR EDI, EDI
			MOV EDI, resim		//Resmin ba�lang�c�ndan bulundu�um yer kadar ilerliyorum
			ADD EDI, EAX
			MOV WORD PTR[EDI], DX//Y���ndan �ekti�im hedef de�erini bulundu�um yere koyup swap� tamaml�yorum.		

			XOR ECX, ECX			//CX de�erini s�f�rl�yorum. 
			XOR EBX, EBX			//EBX de�erini s�f�rl�yorum.
			POP BX					//Y���na att���m i� d�ng� de�i�kenini �ekiyorum b�ylelikle ara i�lemlerde d�ng� de�erim korunmu� oluyor.
			POP CX					//CX i EAX de�erini saklanmak i�in y���na atm��t�m.�ekiyorum. 
			ADD EBX, 2			//�� d�ng� de�erimi 2 artt�r�yorum.
			LOOP Lupp2

			POP CX				//D�� d�ng� de�erini �ekiyorum.
			ADD ESI, 2			//D�� d�ng� indisimi 2 art�r�yorum.

			LOOP Lupp1

			//TRANSPOZE TAMAM.
								//Bu k�s�mda sola �evirmek istedi�imden matrisin ortas�ndaki sat�ra g�re simetri alaca��m.

			//Kullanaca��m registerlar� �nceki de�erleri (kalm�� ise) s�f�rl�yorum.

			XOR EAX, EAX		//EAX :Bulundugum yer olacak.			
			XOR EBX, EBX		//EBX :�� d�ng� de�erim olacak.�lerleyen k�s�mda ise hedef de�erim olacak. 
			XOR ECX, ECX		//ECX :D�ng� de�erim olacak.
			XOR EDX, EDX		//EDX :Tmp olarak kullanaca��m.	
			XOR EDI, EDI		//EDI :Resimin ba�lang�� adresini tutacak.
			XOR ESI, ESI		//ESI :D�� d�ng� de�erim olacak.

			MOV CX, N			//Bu k�s�mda sat�r say�s�n�n yar�s�na kadar gidece�im.
			SHR CX,1			//��nk� sat�rlar�n ortas�na g�re simetri al�yorum.


			Lup1 :
		PUSH CX					//D�� d�ng� de�erimi kaybetmek istemiyorum.Y���na at�ld�.
			MOV CX, N			//�� d�ng� de�erim atand�.T�m s�tunlar gezilecek N atand�.
			XOR EBX, EBX		//�� d�ng� de�erim d�� d�ng� her d�nd���nde s�f�rlanacak. 
			Lup2 :
			XOR EAX, EAX		//Bulundu�um yeri belirleyece�im.
			MOV AX, N			//N*ESI+EBX (N*i+j) bulundu�um yerdir.
			MUL ESI				//N*ESI ger�eklendi
			ADD EAX, EBX		//N*ESI+EBX ger�eklendi.**BURASI BULUNDUGUM YER**

		PUSH CX					//EAX'deki de�eri saklamam gerekecek.D�ng� de�erimi sakl�yorum.

			MOV ECX, EAX		//EAX i�lem yapaca��m i�in buradaki de�erimi ECX de saklad�m.��lemler bittikten sonra tekrar EAX'e at�lacak.

			XOR EAX, EAX

			MOV AX, N		//Sat�r �zerinde i�lem yapaca��m yani arr[i][j]=arr[n-i-1][j] olacak.
			SHL EAX, 1		//Burada word tipinde �al��t���m i�in asl�nda 2*N yapmal�y�m.
			SUB EAX, 2		//-1 ise -2 de�erinde olacak.(word)
			SUB EAX, ESI	//-i de�erini ger�ekledim
		PUSH BX				//EBX hedef de�erimi g�sterecek.D�ng�deki indis de�erini kaybetmemesi i�in y���na at�yorum.
		PUSH SI				//ESI de�i�en indis de�erini yani(n-i-1) i g�sterecek.D�� d�ng� de�erini kaybetmek istemiyorum.
			
			XOR ESI, ESI
			MOV ESI, EAX		//ESI = (n-i-1) ger�eklendi.
			XOR EAX, EAX

			MOV AX, N			//Hedef de�erimi hesaplayaca��m.
			MUL ESI				//Sat�r say�s�yla boyutu �arp�yorum.D�KKAT!:De�i�en sat�r say�s�yla i�lem yap�ld�.[i][j] de�il [n-i-1][j] yani.
			ADD EAX, EBX		//N*ESI+EBX ger�eklendi.
			MOV EBX, EAX		//EBX hedef de�erim.***ARTIK HEDEF DEGER� BELLI**
		POP SI					//SI ile i�im bitti d�� d�ng� de�erimi korumu� oldum.
			XOR EAX, EAX		

			MOV EAX, ECX		//Bulundu�um yeri ECX de korumu�tum.�ekiyorum.
			XOR EDX, EDX		//DX Tmp olarak kullan�lacak.

			MOV EDI, resim		//Resmin ba�lang�c� al�nd�.
			ADD EDI, EBX		//Hedef adresine gidildi.
			MOV DX, WORD PTR[EDI]//Hedefteki de�er Tmp olarak al�nd�..
		PUSH DX					//Hedefteki de�eri y���nda sakl�yorum.
			XOR DX, DX
			XOR EDI, EDI
			MOV EDI, resim		//Resmin ba�lang�� adresine gidildi.
			ADD EDI, EAX		//Bulundugum yere ilerledim.
			MOV DX, WORD PTR[EDI]//Bulundu�um yerdeki de�er Tmp olarak al�nd�.

			XOR EDI, EDI
			MOV EDI, resim		//Resmin ba�lang�� adresine gidildi.
			ADD EDI, EBX		//Hedef konuma ilerledim
			MOV WORD PTR[EDI], DX//Bulundu�um yerden ald���m de�eri hedef de�erine yazd�m swap�n ilk ad�m� ger�ekle�ti.

		POP DX				//Hedeften ald���m tmp de�eri �ekiyorum

			XOR EDI, EDI
			MOV EDI, resim		//Resmin ba�lang�� adresi al�nd�.
			ADD EDI, EAX		//Bulundugum adrese gidildi.
			MOV WORD PTR[EDI], DX//Hedeften ald���m de�er at�ld�.

			XOR ECX, ECX
			XOR EBX, EBX
		POP BX					//�� d�ng� indisim bozulmadan y���ndan al�nd�.
		POP CX					//ECX de EAX korunmu�tu. D�ng� de�erimi geri �ekiyorum.
			ADD EBX, 2			//�� d�ng� indisini art�r�yorum

			LOOP Lup2

			POP CX				//D�� d�ng� de�erim �ekiliyor.
			ADD ESI, 2			//D�� d�ng� indisim art�r�ld�.

			LOOP Lup1

	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ

}