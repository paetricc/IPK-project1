# IPK - Projekt 1

Autor: Tomáš Bártů, xbartu11\
Ak.rok: 2021/2022

---

Server v jazyce C komunikující prostřednictvím protokolu HTTP, který poskytuje různé informace o systému. Server naslouchá na zadaném portu a podle url vraci určité požadované informace ve formě text/plain.

## Přeložení a spuštění serveru

Po zadání těchto příkazů dojde ke spuštění serveru, který bude naslouchat na zadaném portu.
```
$ make
$ ./hinfosvc [číslo portu]
```

## Příklady použití

Předpokládáme že už máme přeložený a spuštěný server. Pro příklad na portu  25252.

*Pozn.: Mimo jiné je možné zaslat dotaz i prostřednictvím webového prohlížeče.*

### 1. Vytížení serveru

Server je spuštěn v jednom terminálu a v druhém zadáme následující příkaz díky, kterému zjistímě aktuální zatížení serveru respektive počítače.
```
$ GET http://localhost:25252/load
$ 12%
```

### 2. Název procesoru

Server je spuštěn na serveru Merlin a z lokálního počítače posíléme následující dotaz díky, kterému zjistíme na jakém procesoru server funguje.
```
$ curl http://merlin.fit.vutbr.cz:25252/cpu-name
$ Intel(R) Xeon(R) Silver 4214R CPU @ 2.40GHz
```

### 3. Doménové jméno

Zde je server vypnut a pomocí následujícího příkazu spustíme server a zároveň zašleme dotaz díky, kterému zjistímé doménove jméno počítače, na kterém se server nachází.\
Zde začne běžet server na pozadí a pro jeho ukončení je třeba využít příkaz kill ve tvaru *kill PID*, kde PID se zobrazí hned po spuštění příkazu a má tvar \[n\] PID.
```
$ ./hinfosvc 25252 & curl http://147.229.220.236:25252/hostname
$ [1] 154492 // PID (číslo procesu)
$ NTB-LNV    //Doménove jméno
```

## Odkazy
Manuálové stánky: https://man7.org/linux/man-pages/index.html \
Výpočet CPU Load: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux \
Markdown: https://www.markdownguide.org/basic-syntax/