# Szkolenie - Programowanie wielowątkowe w C++ #

## Dokumentacja + slajdy

* https://infotraining.bitbucket.io/cpp-thd

## Środowisko (3 opcje do wyboru)

  1. Konfiguracja lokalna

     Jeśli uczestnik chce skonfigurować lokalnie swoje środowisko, to należy zainstalować:
     
     * Dowolny kompilator C++ (gcc, clang, Visual C++) wspierający C++11/14
     * [CMake > 3.15](https://cmake.org/)
       * proszę sprawdzić wersję w lini poleceń

         ```
         cmake --version
         ```

  2. Zdalna maszyna wirtualna (SSH)
     
     * link do rejestracji: https://labs.azure.com/register/rrteny9y
       - uczestnik powinien mieć konto Github lub Microsoft
       - ustawić swoje hasło przy rejestracji

  3. Wirtualna maszyna - VirtualBox 6.1
     * należy zainstalować najnowszą wersję VirtualBoxa ze strony: https://www.virtualbox.org/
     * pobrać obraz wirtualnej maszyny: https://infotrainingszkolenia-my.sharepoint.com/:u:/g/personal/krystian_piekos_infotraining_pl/Ee0fsUNCbBVNvSByty15x44B57C3sqmL05u8KAl-jevMEA?e=rFLVRe
     * zaimportować obraz w VirtualBoxie (menu File/Import Appliance...)
     * uruchomić maszynę:
       - nazwa użytkownika: dev
       - hasło: pwd

## IDE - Visual Studio Code

* [Visual Studio Code](https://code.visualstudio.com/)

### Zainstalować wtyczki C++

* C/C++
* CMakeTools
* Live Share
* W przypadku pracy ze zdalną maszyna przez SSH należy za(re)instalować wtyczki C++ po połączeniu z maszyną.

### Zainstalować wtyczkę do pracy zdalnej (jeśli wybrano korzystanie ze zdalnej maszyny wirtualnej)

* Remote - SSH
  - opis pracy z wtyczką: https://code.visualstudio.com/docs/remote/ssh
  - UWAGA! Windows 10 - [wymagana jest instalacja klienta OpenSSH lub Git for Windows (wcześniejsze wersje Windows)](https://code.visualstudio.com/docs/remote/troubleshooting#_installing-a-supported-ssh-client)
  - sprawdzić, czy klient SSH jest dostępny w terminalu (dodany do zmiennej PATH)
  - przy połączeniu z maszyną zdalną należy kliknąć na link **details** w pop-upie w prawym dolnym rogu. Umożliwi to wprowadzenie ustawionego wcześniej hasła.
