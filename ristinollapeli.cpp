/*	1.TAMK, Ohjelmointikielet, jatko (OJ 5G00BM35) 2014
	2.Ristinollapeli, ristinollapeli.cpp
	3.Tekijä Juho Ylä-Pynnönen 13i220b
	4.juhoyp@gmail.com
	5.Työn tekemiseen kulunut aika noin 30h


						TEHTY VISUAL STUDION 2012 VERSIOLLA
	Aloin mielessäni suunnittelemaan ohjelmaa jo hyvissä ajoin, mutta takarajan
	lähestyttyä päädyin elegantin ratkaisun puuttuessa lähteä toteuttamaan tylsää
	mutta tässä tapauksessa myös varmaa if else ratkaisua. Tehtävää tehdessäni
	huomasin, että se taisi lopulta olla ehkä helpommasta päästä oleva ratkaisu
	käyttäjän syötteiden käsittelyyn. Sillä syötteet ovat hyvin rajalliset. Mikäli 
	komentoja ja ominaisuuksia olisi ollut enemmän ja etenkin pidempiä syötteitä joista
	olisi pitänyt kaivaa erinäisiä osia esille, olisin päätynyt käyttämään string streamia.
	Pisteiden taulukoimiseen käytin toiselle pelaajalle arvoa -1 ja toiselle +1, sillä se 
	huomattavasti suoraviivaistaa voittorivien metsästämistä muuttaen sen simppeliksi
	yhteenlaskuksi.

	No asiaan, Ohjelman sydän löytyy int mainista ja joka kierroksella ohjelma palaa aina
	takaisin sinne. Tämän valitsin ohjelman helpon lopettamisen takia. En uskaltanut ottaa
	riskiä, että jotain aliohjelmia jäisi jonnekkin bittiavaruuteen muistia täyttämään, koska
	viime ohjelman tekemisestä on jo aikaa. Ohjelma oli todella helppo jakaa järkeviin osa-
	kokonaisuuksiin, joita ovat pelikentän piirtäminen, syötteen tulkinta (hieman suttuinen tuli),
	ja syötteen parsimisen jälkeen varsinaisten puhtaiden koordinaattien sovittaminen itse
	pelilautaan (samalla tarkistaen, että tilaa on). Viimeiseksi jäi voittorivin etsintä ja
	se olikin kaikista haastavin osuus ja varmaan näin muistin palailtua siitä tulikin ehkä
	siistein koodinpätkä koko ohjelmassa. Seuraavaksi koitan hahmotella kaaviota siitä kuka kutsuu
	ja mitä.
				
			 -->	1 laudan tulostus
	int main -->	2 voittajan tarkistus (näin saadaan vielä voitto tilanne näkyviin)
			 -->	3 Käsittele syöte -->  käsittele koordinaatit

	Tärkeimmät muuttujat luodaan jo int mainissa, jotta ne on helppo tarvittaessa päivittää ja
	välittää taas eteenpäin toisaalle. Mielestäni ohjelma ei ole niin laaja, että olisi tarvetta
	hajauttaa sitä .h päätteisten tiedostojen alle.

	Melko pienellä parantelulla ohjelmaan lisäisin uudelleen pelaamis mahdollisuuden, muuten perus 
	peli toimii hyvin. Muotoilussa olen pyrkinyt noudattamaan TTY:llä oppimaani hyväksi todettua
	muotoilutapaa. Selkeiden osakokonaisuuksien jaottelu reiluilla väleillä/laittamalla kaarisulut
	omalle rivilleen. Rivien pituudesta ei ollut varmuutta, mutta pyrikimys oli pysyä lähellä
	80 merkin rivipituutta, kommentit pois lukien.
	*/

#include <iostream>
#include <string> 
#include <iomanip>
#include <ctype.h>

using namespace std;

void tulosta_kentta(int kentta[][30], int korkeus, int leveys);
bool tarkista_siirron_laillisuus(int korkeus,int leveys,int kentta[][30],bool& vuorossa); //siirto kentällä? tyhjä ruutu?
bool kasittele_koordinaatti(string,bool&,int[][30]); //parsii käyttäjän syötteet ja antaa sapiskaa huonoista suorituksista
bool tarkista_voittoja(int korkeus,int leveys,int kentta[][30],bool vuorossa); //ehdoton tuomari julistaa voittajan

int main(){
	int korkeus = 20; //Tärkeimmät joka paikassa käytössä olevat muuttujat, eli laudan koko (ei muutettavissa, mutta const herjaa)
	int leveys = 30;
	bool vuorossa = 1; //tämän totuusarvon avulla seurataan kumpi pelaajista on vuorossa
	string syote;      //käyttäjän syöte napataan tähän ja välitetään aliohjelmien käsiteltäväksi
	int kentta[20][30] = {0};	//itse pelilauta luodaan ja alustetaan tässä, pitäisi... 
								//...sijoittaa silmukan sisälle replay mahdollisuutta varten
	cout << "You can quit the game any time with command EXIT" << endl << endl;

	while (true){  //pää silmukka ja ohjelman sydän
	tulosta_kentta(kentta, korkeus, leveys);
	if (tarkista_voittoja(korkeus, leveys, kentta, vuorossa) == true){
		break;} //<-- olisi voinut sijoittaa tuonne if lauseen hännille, mutta koen selkeämmäksi komennon sijoittamisen eri riville...
	cout << "Give the desired coordinates:"; //...etenkin, kun niitä on enemmän.
	getline(cin,syote);
	if (kasittele_koordinaatti(syote, vuorossa, kentta) == false){
		break;}
	}

	system("pause");
	return 0;
}


// seuraavasta aliohjelmasta saatava boolean (false) lopettaa ohjelman kätevästi ja true jaottelee aliohjelman osiin
bool kasittele_koordinaatti(string syote, bool& vuorossa, int kentta[][30]){
	string::size_type pituus = syote.length();
	char koord1; //char muuttujat syötteen pilkkomista varten, koska pystyvät syömään sekä numerot, että kirjaimet
	char koord2;
	char koord3;
	int numcoord1; //myöhemmässä vaiheessa kaikki koordinaatit muutetaan int tyyppisiksi jatko käsittelyn mahdollistamiseksi
	int numcoord2;
	int numcoord3;

	if (syote == "EXIT"){
		return false;}
	else if (syote == "exit"){ //ensimmäisenä tulee exit käsky, koska se on selkeä sana ja sitä on turha lähteä pilkkomaan
		return false;}
	else if (syote.empty())	{ //mikäli käyttäjä painoi vahingossa enteriä, on turha jatkaa
	cout << "You didn't give any coordinates!" << endl;
	return true;}
	else if (pituus > 3){ //syötteen pituuden pitää olla joko 2, tai 3 muuten ei taaskaan kannata jatkaa
		cout << "You didn't give valid coordinates!" << endl;
		return true;} 
	else if (pituus < 2){ //virheilmoitukset ovat hieman geneerisiä, mutta ainakin pitävät paikkansa
	cout << "You didn't give valid coordinates!" << endl;
		return true;}

	else { //alun karsinnat on tehty ja nyt voimme alkaa pilkkoa selvinnyttä osaa ja katsoa mitä se on syönyt
		koord1 = syote.at(0);		//ainakin nämä kaksi koord1 ja koord2 pitää tässä vaiheessa olla ja vähempää tänne ei päästetäkkään
		koord2 = syote.at(1);
				if (isalpha(koord1) == 0 || isdigit(koord2) == 0){ //molempien pitää olla määrättyä muotoa, tai jälleen turha jatkaa
		cout << "Wrong coordinates!" << endl;
	return true;}
		numcoord2 = koord2 - 48;
	if (pituus > 2){	//kolmannen koordinaatin kanssa jouduin hieman kikkailemaan, koska se saattaa olla olemassa, tai sitten ei...
		cout << syote.at(2) << endl;  //... ja mikäli se löytyy, muuttaa se ensimmäisen koordinaatin merkityksen kertoimella 10
		koord3 = syote.at(2);
		numcoord3 = koord3 - 48;
		cout << numcoord3 << "koordi3" << endl;
		numcoord2 = 10*numcoord2+numcoord3; //kerro, leikkaa, liimaa ja näin meillä on kymmenykset mukana, mikäli niitä löytyi
		cout << numcoord2 << "koordi2" << endl;
		if (isdigit(koord3) == 0){ //myös kolmannen koordinaatin täytyy olla digitti
			cout << "Wrong coordinates edit3!" << endl;
	return true;}
	}
	}	
	koord1 = toupper(koord1); //varmistetaan, että kirjain on iso, jotta päästään vähemmällä numeromuunnoksessa
	numcoord1 = koord1 - 'A';
	numcoord2 = numcoord2 - 1; //koska numerot eivät ala nollasta, pitää se hoitaa käyttäjän puolesta taulukkovaihetta helpottaaksemme
	cout << koord1 << endl;
	cout << koord2 << endl;
	cout << numcoord1 << endl; //nyt meillä on täysin 100% puhtaat ja siistit koordinaatit, joita on mukava käsitellä jatkossa
	cout << numcoord2 << endl; //jätetään koordinaattien osumisen tarkistelu sinne minne se kuuluukin, eli sijoitus osioon
	if (tarkista_siirron_laillisuus(numcoord1, numcoord2, kentta, vuorossa) == 0){
		return true;}
}


//kentän tulostaminen on suoraviivaista puuhaa ja pienehköllä muokkaamisella sen saisi myös säätymään koon mukana, nyt se ei ollut tarpeen
void tulosta_kentta( int kentta[][30], int korkeus, int leveys){
	cout << setw(34) << setfill(' ') << "           1         2         3  " << endl;
	cout << setw(34) << "  123456789012345678901234567890  " << endl;
	cout << "  ";
	cout << setfill('-') << setw(30) << "-" << setfill(' ') << endl;
	for (int i = 0; i < korkeus; ++i) {		//for silmukoiden avulla tutkitaan taulukon sisältöä ja tulostellaan mahdolliset merkit ja...
		int apumuuttuja = 65+i;				//... muuten tyhjää -1 taulukossa on O ja +1 esittää X:ää	
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


//vielä tutkitaan onko koordinaatteja olemassa ja onko paikka jo mahdollisesti viety, true vie peliä eteenpäin ja samalla vaihdetaan pelaajaa
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
	kentta[korkeus][leveys] = -1; //merkintä pistetaulukkoon
	vuorossa = true;  //vuorossa olevan pelaajan vaihto
	return true;}
	else {
	kentta[korkeus][leveys] = 1;
	vuorossa = false;
	return true;}
}


//tutkitaan joka kierroksella kaikki mahdolliset voittolinjat, ensin vaaka ja sitten pystyrivit, lopuksi jäivät vinottaiset
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
				
	for(int i = 4; i < leveys; ++i){	//tämä for huolehtii pistelaskun sivuttais liikkeestä
		int x = -1;
		for(int b = i; b > 3; --b){ //tämä for huolehtii pistelaskun sivuttais liikkeestä
			int apulaskuri = 0;		
			++x;	//apumuuttuja pystysuunnassa liikkumiseen, alla oleva if rajoittaa liikkeen alareunaan	
			if (x == 16){--x;}
			for (int c = 0; c < 5; ++c){ // tämän kaavan avulla pistelaskuri liikkee sivuttain summasen 5 palan pätkiä, jonka jälkeen palataan ylös
				apulaskuri = apulaskuri + kentta[x+c][i-c]; 
				if (apulaskuri == 5){cout << "Player X wins!" << endl;
				return true;}
				if (apulaskuri == -5){cout << "Player O wins!" << endl;
				return true;}
						}}}

	for(int i = leveys-4; i > 0; --i){	//tämä osio on yläpuolella olevan peilikuva
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
		
	return false; //ei voittoa, kaikki true arvot vievät pelin lopetukseen ja voittaja julistetaan suoraan täältä käsin.
		}


//kello on 1:23, sähköpostia ja nukkumaan.