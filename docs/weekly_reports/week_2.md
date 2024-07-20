# Viikkoraportti 2

Tuntimäärä: ≈12

Viikolla 2 luin lisää Jump Point Search -algoritmin toteutuksesta ja periaatteesta.
Olin lukenut ja perehtynyt siihen jo viikolla 1, mutta nyt keskityin enemmän tekniseen toteutukseen.
Resursseina toimivat alkuperäinen [Online Graph Pruning for Pathfinding on Grid Maps](https://users.cecs.anu.edu.au/~dharabor/data/papers/harabor-grastien-aaai11.pdf), mutta myös [The JPS Pathfinding System](https://ojs.aaai.org/index.php/SOCS/article/view/18254/18045) sekä [JPS+: An Extreme A* Speed Optimization for Static Uniform Cost Grids](http://www.gameaipro.com/GameAIPro2/GameAIPro2_Chapter14_JPS_Plus_An_Extreme_A_Star_Speed_Optimization_for_Static_Uniform_Cost_Grids.pdf).

Ohjelman tasolla paransin useiden funktioiden ja ominaisuuksien uudelleenkäytettävyyttä erittelemällä ne A*:n toteutuksesta yleiseen Util-nimiavaruuteen. Toteutin myös JPS-algoritmin A*:n kanssa koska tajusin, että JPS ei ole itsessään reitinhakualgoritmi vaan enneminkin tutkittavan alueen ja symmetrian karsimisalgoritmi.

Olettaen, että A*- ja JPS-algoritmit toimivat oikein (mikä näyttää silmämääräisesti arvioiden pitävän paikkaansa; ne myös palauttavat samoista kartoista samat optimaalisilta näyttävät etäisyydet), sovelluksen logiikka on siis valmis. Ainoastaan testit puuttuvat, joita alankin seuraavalla viikolla heti työstämään.

JPS-algoritmiin liittyen minulla on kuitenkin vielä muutamia kysymyksiä: haittaako, vaikka otin muutamia luovia vapauksia ja mm. skippasin kokonaan JPS:n `jump`-funktiossa diagonaalisesti etenevien noodien lisäämisen erikseen laajennettaviksi jump pointeiksi? (niitä ei siis koskaan lisätä avoimeen prioriteettijonoon, ainoastaan kohtisuoraan etenevät solmut lisätään, jos niillä on "pakotettuja naapureita")
Ainakin nopeasti päätellen tämän ei pitäisi muuttaa reitinhaun optimaalisuutta. 

Toinen kysymys koskee visualisointia: tällä hetkellä visualisointi päivittää näyttöä joka kerta, kun yksi prioriteettijonon solmu on saatu laajennettua ja käsiteltyä läpi. A*-algoritmissa tämä toimii hyvin, mutta koska JPS-algoritmin kanssa prioriteettijonossa on niin paljon vähemmän solmuja, varsinkin todella yksinkertaisissa kartoissa visualisaatio tapahtuu todella nopeissa pyrähdyksissä. Monimutkaisemmissa kartoissa (esimerkiksi `data`-kansion kartassa) visualisointi on asteittaisempaa. Onko tämä kurssille riittävää?