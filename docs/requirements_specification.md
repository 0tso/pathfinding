# Requirements specification

The requirements specification is provided in Finnish for the course _Aineopintojen harjoitustyö: Algoritmit ja tekoäly_ of the University of Helsinki.

<hr>

Opinto-ohjelma: tietojenkäsittelytieteen kandiohjelma

Projektin kieli on yleisesti englanti, mutta määrittelydokumentti sekä mahdollisesti jokin muu osa (koodin ulkoisesta) dokumentaatiosta kirjoitetaan suomeksi.

Käytetty ohjelmointikieli: C++

Muut hallitut ohjelmointikielet: C, Python, Javascript. Tämän lisäksi Java ja C# varmaan luettavissa koska syntaksiltaan läheisiä C++:aan ja olen joskus aikoinani niihin pinnallisesti perehtynyt.

Toteutan työssä A* ja jump point search (JPS) -algoritmit. Vertailen algoritmien tehokkuutta ja esitän niiden toiminnan visuaalisesti kaksiuloitteisella neliöpohjaisella kartalla.

Algoritmit ratkaisevat reitinhakuongelman: kuinka löytää lyhyin ja tehokkain reitti kohteesta A kohteeseen B verkossa. Jump point search -algoritmi käyttää hyväkseen erityisesti tasaisten, ruudukkomaisten verkkojen erityisominaisuuksia, eli se soveltuu esimerkiksi peleihin. Oma motivaatio reitinhakualgoritmien implementoimiseen tuleekin juuri pelien vaatimuksista.

Pelifokuksen ja JPS-algoritmin käytön takia verkon kaikkien reunojen hinta / paino on sama. Algoritmeista tarjotaan niin neljään suuntaan kuin kahdeksaan suuntaan etenevät versiot.

Pääohjelma on visuaalinen: se näyttää käyttäjälle kartan, johon käyttäjä voi piirtää esteitä sekä asettaa lähtö- ja loppupisteet. Käyttäjä voi myös lopulta konsoliin tietyt komennot syöttämällä suorittaa halutun reitinhakualgoritmin. Suorittaessa & suorituksen jälkeen ohjelma näyttää visuaalisesti kiinnostavia reitinhaun ominaisuuksia (esimerkiksi löydetty polku, tutkitut solmut, jne.) Kartta voidaan myös ladata ja tulkita suoraan kuvatiedostosta, joka ohjelmalle voidaan antaa komentoriviparametrina.

Visuaalisen pääohjelman lisäksi voidaan komentoriviltä suorittaa testiohjelma, joka suorittaa yksikkötestit, tarkistaa algoritmien paikkaansapitävyyden ja mittaa useassa eri "skenaariossa" A* ja JPS -algoritmien nopeutta.

A*-algoritmin huonoimman tilanteen aikavaatimus on O(b^d), jossa `b` on haarautumiskerroin eli solmun "synnyttämien" jälkeläisten keskimääräinen lukumäärä, ja `d` on lyhyimmän polun pituus. Tilavaatimus on samanlainen.

Jump point search -algoritmi toimii oletuksella, että verkon solmujen suora läpikäyminen on nopeampaa ja tehokkaampaa kuin niiden tallentaminen prioriteettijonoon kuten A*-algoritmissa (olettamus, joka yleisesti pätee, koska prioriteettijonon ylläpitämiseksi alkion lisäämisen aikavaatimus on O(log n)). JPS-algoritmin tilavaatimus on siksi yleisessä tapauksessa paljon A*-algoritmia pienempi ja nopeus suurempi. Huonoimmassa tapauksessa (esimerkiksi tiheässä sokkelossa, jossa lähes jokainen solmu on _jump point_) tilavaatimus ja varsinkin aikavaatimus voivat kuitenkin varmaankin olla varsin lähellä A*-algoritmia.

### Viitteet

https://en.wikipedia.org/wiki/A*_search_algorithm

https://en.wikipedia.org/wiki/Jump_point_search

https://zerowidth.com/2013/a-visual-explanation-of-jump-point-search/

http://users.cecs.anu.edu.au/~dharabor/data/papers/harabor-grastien-aaai11.pdf


### Muita reitinhakuun liittyviä algoritmeja ja ideoita tutkittavaksi, jos aikaa jää

* A* jaetun prioriteettijonon kanssa (jono jaetaan jatkuvasti järjestyksessä pidettäviin ja muihin)
* Fringe search
* Flow fields
* Rectangular symmetry reduction
* D*
* Navigation meshes
* Collaborative diffusion
* B*
* Any-angle path planning
* Quad trees
* Swamps