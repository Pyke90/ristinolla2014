/*	1.TAMK, Ohjelmointikielet, jatko (OJ 5G00BM35) 2014
	2.Ristinollapeli, ristinollapeli.cpp
	3.Tekij� Juho Yl�-Pynn�nen 13i220b
	4.juhoyp@gmail.com
	5.Ty�n tekemiseen kulunut aika noin 30h


						TEHTY VISUAL STUDION 2012 VERSIOLLA
	Aloin mieless�ni suunnittelemaan ohjelmaa jo hyviss� ajoin, mutta takarajan
	l�hestytty� p��dyin elegantin ratkaisun puuttuessa l�hte� toteuttamaan tyls��
	mutta t�ss� tapauksessa my�s varmaa if else ratkaisua. Teht�v�� tehdess�ni
	huomasin, ett� se taisi lopulta olla ehk� helpommasta p��st� oleva ratkaisu
	k�ytt�j�n sy�tteiden k�sittelyyn. Sill� sy�tteet ovat hyvin rajalliset. Mik�li 
	komentoja ja ominaisuuksia olisi ollut enemm�n ja etenkin pidempi� sy�tteit� joista
	olisi pit�nyt kaivaa erin�isi� osia esille, olisin p��tynyt k�ytt�m��n string streamia.
	Pisteiden taulukoimiseen k�ytin toiselle pelaajalle arvoa -1 ja toiselle +1, sill� se 
	huomattavasti suoraviivaistaa voittorivien mets�st�mist� muuttaen sen simppeliksi
	yhteenlaskuksi.

	No asiaan, Ohjelman syd�n l�ytyy int mainista ja joka kierroksella ohjelma palaa aina
	takaisin sinne. T�m�n valitsin ohjelman helpon lopettamisen takia. En uskaltanut ottaa
	riski�, ett� jotain aliohjelmia j�isi jonnekkin bittiavaruuteen muistia t�ytt�m��n, koska
	viime ohjelman tekemisest� on jo aikaa. Ohjelma oli todella helppo jakaa j�rkeviin osa-
	kokonaisuuksiin, joita ovat pelikent�n piirt�minen, sy�tteen tulkinta (hieman suttuinen tuli),
	ja sy�tteen parsimisen j�lkeen varsinaisten puhtaiden koordinaattien sovittaminen itse
	pelilautaan (samalla tarkistaen, ett� tilaa on). Viimeiseksi j�i voittorivin etsint� ja
	se olikin kaikista haastavin osuus ja varmaan n�in muistin palailtua siit� tulikin ehk�
	siistein koodinp�tk� koko ohjelmassa. Seuraavaksi koitan hahmotella kaaviota siit� kuka kutsuu
	ja mit�.
				
			 -->	1 laudan tulostus
	int main -->	2 voittajan tarkistus (n�in saadaan viel� voitto tilanne n�kyviin)
			 -->	3 K�sittele sy�te -->  k�sittele koordinaatit

	T�rkeimm�t muuttujat luodaan jo int mainissa, jotta ne on helppo tarvittaessa p�ivitt�� ja
	v�litt�� taas eteenp�in toisaalle. Mielest�ni ohjelma ei ole niin laaja, ett� olisi tarvetta
	hajauttaa sit� .h p��tteisten tiedostojen alle.

	Melko pienell� parantelulla ohjelmaan lis�isin uudelleen pelaamis mahdollisuuden, muuten perus 
	peli toimii hyvin. Muotoilussa olen pyrkinyt noudattamaan TTY:ll� oppimaani hyv�ksi todettua
	muotoilutapaa. Selkeiden osakokonaisuuksien jaottelu reiluilla v�leill�/laittamalla kaarisulut
	omalle rivilleen. Rivien pituudesta ei ollut varmuutta, mutta pyrikimys oli pysy� l�hell�
	80 merkin rivipituutta, kommentit pois lukien.
	*/

#include <iostream>
#include <string> 
#include <iomanip>
#include <ctype.h>

using namespace std;

void tulosta_kentta(int kentta[][30], int korkeus, int leveys);
bool tarkista_siirron_laillisuus(int korkeus,int leveys,int kentta[][30],bool& vuorossa); //siirto kent�ll�? tyhj� ruutu?
bool kasittele_koordinaatti(string,bool&,int[][30]); //parsii k�ytt�j�n sy�tteet ja antaa sapiskaa huonoista suorituksista
bool tarkista_voittoja(int korkeus,int leveys,int kentta[][30],bool vuorossa); //ehdoton tuomari julistaa voittajan

int main(){
	int korkeus = 20; //T�rkeimm�t joka paikassa k�yt�ss� olevat muuttujat, eli laudan koko (ei muutettavissa, mutta const herjaa)
	int leveys = 30;
	bool vuorossa = 1; //t�m�n totuusarvon avulla seurataan kumpi pelaajista on vuorossa
	string syote;      //k�ytt�j�n sy�te napataan t�h�n ja v�litet��n aliohjelmien k�sitelt�v�ksi
	int kentta[20][30] = {0};	//itse pelilauta luodaan ja alustetaan t�ss�, pit�isi... 
								//...sijoittaa silmukan sis�lle replay mahdollisuutta varten
	cout << "You can quit the game any time with command EXIT" << endl << endl;

	while (true){  //p�� silmukka ja ohjelman syd�n
	tulosta_kentta(kentta, korkeus, leveys);
	if (tarkista_voittoja(korkeus, leveys, kentta, vuorossa) == true){
		break;} //<-- olisi voinut sijoittaa tuonne if lauseen h�nnille, mutta koen selke�mm�ksi komennon sijoittamisen eri riville...
	cout << "Give the desired coordinates:"; //...etenkin, kun niit� on enemm�n.
	getline(cin,syote);
	if (kasittele_koordinaatti(syote, vuorossa, kentta) == false){
		break;}
	}

	system("pause");
	return 0;
}


// seuraavasta aliohjelmasta saatava boolean (false) lopettaa ohjelman k�tev�sti ja true jaottelee aliohjelman osiin
bool kasittele_koordinaatti(string syote, bool& vuorossa, int kentta[][30]){
	string::size_type pituus = syote.length();
	char koord1; //char muuttujat sy�tteen pilkkomista varten, koska pystyv�t sy�m��n sek� numerot, ett� kirjaimet
	char koord2;
	char koord3;
	int numcoord1; //my�hemm�ss� vaiheessa kaikki koordinaatit muutetaan int tyyppisiksi jatko k�sittelyn mahdollistamiseksi
	int numcoord2;
	int numcoord3;

	if (syote == "EXIT"){
		return false;}
	else if (syote == "exit"){ //ensimm�isen� tulee exit k�sky, koska se on selke� sana ja sit� on turha l�hte� pilkkomaan
		return false;}
	else if (syote.empty())	{ //mik�li k�ytt�j� painoi vahingossa enteri�, on turha jatkaa
	cout << "You didn't give any coordinates!" << endl;
	return true;}
	else if (pituus > 3){ //sy�tteen pituuden pit�� olla joko 2, tai 3 muuten ei taaskaan kannata jatkaa
		cout << "You didn't give valid coordinates!" << endl;
		return true;} 
	else if (pituus < 2){ //virheilmoitukset ovat hieman geneerisi�, mutta ainakin pit�v�t paikkansa
	cout << "You didn't give valid coordinates!" << endl;
		return true;}

	else { //alun karsinnat on tehty ja nyt voimme alkaa pilkkoa selvinnytt� osaa ja katsoa mit� se on sy�nyt
		koord1 = syote.at(0);		//ainakin n�m� kaksi koord1 ja koord2 pit�� t�ss� vaiheessa olla ja v�hemp�� t�nne ei p��stet�kk��n
		koord2 = syote.at(1);
				if (isalpha(koord1) == 0 || isdigit(koord2) == 0){ //molempien pit�� olla m��r�tty� muotoa, tai j�lleen turha jatkaa
		cout << "Wrong coordinates!" << endl;
	return true;}
		numcoord2 = koord2 - 48;
	if (pituus > 2){	//kolmannen koordinaatin kanssa jouduin hieman kikkailemaan, koska se saattaa olla olemassa, tai sitten ei...
		cout << syote.at(2) << endl;  //... ja mik�li se l�ytyy, muuttaa se ensimm�isen koordinaatin merkityksen kertoimella 10
		koord3 = syote.at(2);
		numcoord3 = koord3 - 48;
		cout << numcoord3 << "koordi3" << endl;
		numcoord2 = 10*numcoord2+numcoord3; //kerro, leikkaa, liimaa ja n�in meill� on kymmenykset mukana, mik�li niit� l�ytyi
		cout << numcoord2 << "koordi2" << endl;
		if (isdigit(koord3) == 0){ //my�s kolmannen koordinaatin t�ytyy olla digitti
			cout << "Wrong coordinates edit3!" << endl;
	return true;}
	}
	}	
	koord1 = toupper(koord1); //varmistetaan, ett� kirjain on iso, jotta p��st��n v�hemm�ll� numeromuunnoksessa
	numcoord1 = koord1 - 'A';
	numcoord2 = numcoord2 - 1; //koska numerot eiv�t ala nollasta, pit�� se hoitaa k�ytt�j�n puolesta taulukkovaihetta helpottaaksemme
	cout << koord1 << endl;
	cout << koord2 << endl;
	cout << numcoord1 << endl; //nyt meill� on t�ysin 100% puhtaat ja siistit koordinaatit, joita on mukava k�sitell� jatkossa
	cout << numcoord2 << endl; //j�tet��n koordinaattien osumisen tarkistelu sinne minne se kuuluukin, eli sijoitus osioon
	if (tarkista_siirron_laillisuus(numcoord1, numcoord2, kentta, vuorossa) == 0){
		return true;}
}


//kent�n tulostaminen on suoraviivaista puuhaa ja pienehk�ll� muokkaamisella sen saisi my�s s��tym��n koon mukana, nyt se ei ollut tarpeen
void tulosta_kentta( int kentta[][30], int korkeus, int leveys){
	cout << setw(34) << setfill(' ') << "           1         2         3  " << endl;
	cout << setw(34) << "  123456789012345678901234567890  " << endl;
	cout << "  ";
	cout << setfill('-') << setw(30) << "-" << setfill(' ') << endl;
	for (int i = 0; i < korkeus; ++i) {		//for silmukoiden avulla tutkitaan taulukon sis�lt�� ja tulostellaan mahdolliset merkit ja...
		int apumuuttuja = 65+i;				//... muuten tyhj�� -1 taulukossa on O ja +1 esitt�� X:��	
		cout << (char) apumuuttuja << "|";
   
		for (int b = 0; b < leveys; ++b) {
			if (kentta[i][b] == 1){
			cout << "X";}
			else if (kentta[i][b] == -1){
			cout << "O";}
			else{cout << " ";}
		}
		cout << "|" << (char) apumuuttuja << endl;
}
	cout << "  ";
	cout << setfill('-') << setw(30) << "-" << setfill(' ') << endl;
	cout << setw(34) << "  123456789012345678901234567890  " << endl;
	cout << setw(34) << setfill(' ') << "           1         2         3  " << endl;
}


//viel� tutkitaan onko koordinaatteja olemassa ja onko paikka jo mahdollisesti viety, true vie peli� eteenp�in ja samalla vaihdetaan pelaajaa
bool tarkista_siirron_laillisuus(int korkeus,int leveys,int kentta[][30],bool& vuorossa){
	if (korkeus > 20){
		cout << "Given coordinates doesn't exist" << endl;
		return false;}
	else if (leveys > 29){
		cout << "Given coordinates doesn't exist" << endl;
		return false;}
	else if (leveys < 0){
		cout << "Given coordinates doesn't exist" << endl;
		return false;}
	else if (kentta[korkeus][leveys] != 0){
		cout << "Given coordinates are already taken, please try again." << endl;
	return false;
	}
	if (vuorossa == 0){
	kentta[korkeus][leveys] = -1; //merkint� pistetaulukkoon
	vuorossa = true;  //vuorossa olevan pelaajan vaihto
	return true;}
	else {
	kentta[korkeus][leveys] = 1;
	vuorossa = false;
	return true;}
}


//tutkitaan joka kierroksella kaikki mahdolliset voittolinjat, ensin vaaka ja sitten pystyrivit, lopuksi j�iv�t vinottaiset
bool tarkista_voittoja(int korkeus,int leveys,int kentta[][30],bool vuorossa){
	
	for(int i = 0; i < korkeus; ++i){	
		for(int b = 0; b < leveys-4; ++b){
			int apulaskuri = 0;		
			for (int c = 0; c < 5; ++c){
				apulaskuri = apulaskuri + kentta[i][b+c];
				if (apulaskuri == 5){cout << "Player X wins!" << endl;
				return true;}
				if (apulaskuri == -5){cout << "Player O wins!" << endl;
				return true;}
						}}}

	for(int i = 0; i < leveys; ++i){		
		for(int b = 0; b < korkeus-4; ++b){
			int apulaskuri = 0;			
			for (int c = 0; c < 5; ++c){
				apulaskuri = apulaskuri + kentta[b+c][i];			
				if (apulaskuri == 5){cout << "Player X wins!" << endl;
				return true;}
				if (apulaskuri == -5){cout << "Player O wins!" << endl;
				return true;}
						}}}
				
	for(int i = 4; i < leveys; ++i){	//t�m� for huolehtii pistelaskun sivuttais liikkeest�
		int x = -1;
		for(int b = i; b > 3; --b){ //t�m� for huolehtii pistelaskun sivuttais liikkeest�
			int apulaskuri = 0;		
			++x;	//apumuuttuja pystysuunnassa liikkumiseen, alla oleva if rajoittaa liikkeen alareunaan	
			if (x == 16){--x;}
			for (int c = 0; c < 5; ++c){ // t�m�n kaavan avulla pistelaskuri liikkee sivuttain summasen 5 palan p�tki�, jonka j�lkeen palataan yl�s
				apulaskuri = apulaskuri + kentta[x+c][i-c]; 
				if (apulaskuri == 5){cout << "Player X wins!" << endl;
				return true;}
				if (apulaskuri == -5){cout << "Player O wins!" << endl;
				return true;}
						}}}

	for(int i = leveys-4; i > 0; --i){	//t�m� osio on yl�puolella olevan peilikuva
		int x = -1;
		for(int b = i; b < 30; ++b){
			int apulaskuri = 0;		
			++x;		
			if (x == 16){--x;}
			for (int c = 0; c < 5; ++c){
				apulaskuri = apulaskuri + kentta[x+c][i+c];
				if (apulaskuri == 5){cout << "Player X wins!" << endl;
				return true;}
				if (apulaskuri == -5){cout << "Player O wins!" << endl;
				return true;}
						}}}
		
	return false; //ei voittoa, kaikki true arvot viev�t pelin lopetukseen ja voittaja julistetaan suoraan t��lt� k�sin.
		}


//kello on 1:23, s�hk�postia ja nukkumaan.