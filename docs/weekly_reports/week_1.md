# Viikkoraportti 1

Tuntimäärä: ≈20

Viikolla 1 varmistuin valitsemastani aiheesta aloitusluennon aikana. Tämän jälkeen alustin projektin repositorion, opiskelin C++ paketinhallintatyökalujen käyttöä lähinnä vcpkg:n kautta (joka osoittautui huomattavasti esimerkiksi Pythonin Poetry-työkalua kömpelömmäksi),  ja laitoin projektin buildaamiseen tarvittavat asiat ja README:n kuntoon. Kirjoitin myös tietenkin määrittelydokumentin sekä lopuksi myös alustavat käyttöohjeet.

Itse ohjelman osalta tein lähes kaiken algoritmien ulkoisen koodin rakenteen valmiiksi (renderöinnin, käyttäjän syötteen hallinnan, karttatiedostojen lukemisen levyltä) sekä implementoin A*-algoritmin. Ainakin alustavasti näyttää siltä, että algoritmi toimii kuten pitääkin.

Ensimmäisen viikon edistyksen sanoisin siis olevan varsin hyvä. Vaikeuksia tuottivat lähinnä graafinen hienosäätö, esimerkiksi SFML:n ikkunan toimintaan perehtyminen. Pikselien kanssa temppuilu tuotti omat hankaluutensa.

Kurssin osalta puuttuvat vielä toinen vertailtavista algoritmeista (JPS), testaaminen ja muu dokumentointi. Seuraavana implementoin JPS-algoritmin, ja tutkin sitten tarkemmin, miten algoritmeja kuuluisi oikeaoppisesti testata ja dokumentoida.

Kysymyksenä voisin esittää: onko tähän mennessä tehty riittävän tasoista loppupalautuksen kannalta? Esimerkiksi onko [algorithms](../../src/algorithms/)-kansiossa olevan koodin (eli kurssilla arvosteltavan logiikan) kommentointi riittävää? Vastaako ohjelman toiminta ([käyttöohjeiden](../usage_instructions.md) mukaan suoritettuna) kurssin vaatimuksia yleisesti?