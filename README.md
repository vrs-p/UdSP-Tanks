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
Následne sa hráč presunie do klientského adresára s názvom ***client-side***. Na spustenie klientskej časti stačí mať nejaké vývojové prostredie (v našom prípade CLion), cez ktoré si spustíme danú časť aplikácie.

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
Server je ovládaný pomocou server_controller-a. Pomocou neho môže hráč vytvárať a pripájať sa na servery alebo môže vypnúť aj celý server, ak na ňom nie sú žiadné aktívne hry a hráči.<br />

### Spustenie servera
Rovnako ako pri klientovi aj server vyžaduje pre svoje fungovanie CSFML knižnicu, pretože ju využívame pri vzájomnej komunikácií. Rovnako ako pri klientovi, tak je možné spustiť serverovú časť aplikácie cez vývojové prostredie alebo pomocou Makefile.<br>

```
Spustenie pomocou terminálu a Makefilu
    - Otvoríme terminálove okno
    - Prejdeme do adresára s názvom server-side
    - Napišeme príkaz: make
    - Následne len spustíme vytvorený súbor pomocou príkazu: ./server
    - Prajeme príjemne hranie :)
```