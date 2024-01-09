# UdSP-Tanks


## Client
Klientská aplikácia zobrazuje grafické okno a stará sa o komunikáciu so serverom.<br />
Po spustení aplikácie sa zobrazí grafické okno, kde má hráč možnosť vytvoriť server (tu ho aplikácia aj automaticky pripojí do danej hry), pripojiť sa k nejakej hre alebo vypnúť server. Taktiež sa hráčovi v tomto okne zobrazuje počet aktívných hier a hráčov. V prípade, že nie je zapnutý server zobrazí sa namiesto týchto informácií chybová hláška a znemožní sa použitie tlačidiel.<br>
Po zobrazení okien na vytvorenie servera alebo pripojenie sa do hry sa hráčovi zobrazia textové polia, do ktorých zadá potrebné údaje. Medzi poliami sa naviguje pomocou šípok.<br>
Pre naklonovanie repozitára musí hrač spraviť:
```
mkdir udsp-tanks
git clone https://gitlab.com/LogiMou/udsp-tanks.git
```
Následne sa hráč presunie do klientského adresára s názvom ***client-side***. Nižšie sú uvedené kroky, ktoré sú potrebné na to, aby bolo možné spustiť klientskú časť.
### MinGW-64 - TODO!!!!!!!!!!
TERAZ JE ASI TEORETICKY JEDNO AKÝ JE KOMPILÁTOR, ČI? Ja mám build tool ninja a kompilátor cc<BR><BR>
Klient beží na kompilátore **MinGW-64**. Preto odporúčamé pred spustením hry mať tento kompílátor nainštalovaný.
Sú rôzne cesty ako nainštalovať MinGW-64. Spomenieme 2 možnosti, ktoré sme využili my. Ak budete postupovať pomocou druhého
spôsobu, tak si zároveň aj nainštalujete ***make*** pomocou ktorého si budete môcť vyskúšat spustiť Clientov **MakeFile**.

```
Celkovo neviem, či tu vôbec tieto dve časti nechávať
1. Stiahnuť Clion, kde sa natívne nainštaluje MinGW-64
2. Postupovať podľa tohto tutoriálu: https://www.youtube.com/watch?v=Zcy981HhGw0&ab_channel=SemicolonDotDev.
```

### CSFML knižnica
V tejto časti ukážeme ako nainštalovať **CSMFL** knižnicu, ktorá je potrebná pre beh hry. Z tejto knižnice využívame grafické časti
a tak isto aj sieťové komponenty, pomocou ktorých prebieha komunikácia medzi serverom a klientom. Nižšie ukážeme step-by-step postup ako si nainštalovať **CSFML** knižnicu na OS Fedora.

```
1. Stiahnút si nasledujúce knižnice cez sudo dnf install: libXrandr-devel libXrandr systemd-devel glew-devel glew libjpeg-turbo-devel libjpeg-turbo-utils libjpeg-turbo turbojpeg turbojpeg-devel SDL_ttf SDL_ttf-devel freetype freetype-devel libsndfile-devel libsndfile libsndfile-utils freealut freealut-devel
2. Stiahnúť si SFML knižnicu pomocou: wget https://www.sfml-dev.org/files/SFML-2.5.1-sources.zip
3. Rozbaliť si stiahnutý súbor: unzip SFML-2.5.1-sources.zip
4. Vykonať nasledujúce príkazy:
        cd SFML-2.5.1/
        cmake .
        cmake --build .
        sudo make install
5. Stiahnúť knižnice CSFML a CSFML-devel
6. Teraz už len stačí reloadnuť CMakeList a skompilovať projekt
7. Prajeme príjemné hranie :)
```

## Server - TODO!!!!!!!!
Serverová časť sa stará komúnikáciu s klientmi. Čaká na pripojenie sa všetkých hráčov, posiela začiatočné nastavenia hráčom (ich ID, súradnice, kde sa majú postaviť, ...). Zapisuje skóre a po ukončení hry rozpošle hráčom ich štatistiky.<br />
Po spustení servera je vyžiadaný vstup od administrátora servera pre počet hráčov (koľko hráčov bude v danej hre). Následne, keď do konzoly zadá počet hráčov, tak sa čaká na ich pripojenie. Po ich pripojení sa rozpošlú inicializačné nastavenia a spustí sa hra.<br />

### Spustenie servera
Rovnako ako pri klientovi aj server vyžaduje pre svoje fungovanie CSFML knižnicu, pretože ju využívame pri vzájomnej komunikácií.<br>
<br>TODO!!!!!!!!!!!!!! Tu tiež v podstate stačí mať CSFML na pc a iba spustiť server, nie?<br> Predpokladáme, že repozitár je naklonovaný a že serverová aplikácia beží na ***frios2.fri.uniza.sk*** servery. Ak sú všetky tieto podmienky splnené, tak už netreba nič nastavovať a len spustiť server.<br />
Dá sa to pomocou dvoch spôsobov:
```
TODO!!!!!!!!!!
1. Pomocou terminálu a Makefilu
    - Otvoríme terminálove okno
    - Prejdeme do adresára s názvom server
    - Napišeme príkaz **make**
    - Následne len spustíme vytvorený súbor pomocou príkazu **./server**
    - Prajeme príjemne hranie :)
2. Pomocou Clionu
    - Na Clione mame nastavený remote development
    - Otvoríme adresár server pomocou Clionu
    - Počkáme, kým sa prenesú súbory na server
    - Reloadneme CmakeList
    - Spustíme aplikáciu
    - Prajeme príjemné hranie :)
```