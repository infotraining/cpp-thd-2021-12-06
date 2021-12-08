# Szkolenie - Programowanie wielowątkowe w C++ #

## Linki

* C/C++11 mappings to processors - https://www.cl.cam.ac.uk/~pes20/cpp/cpp0xmappings.html
* Atomic weapons
  * https://www.youtube.com/watch?v=A8eCGOqgvH4&ab_channel=toalexsmail
  * https://www.youtube.com/watch?v=KeLBd2EJLOU&ab_channel=toalexsmail
* Weakly ordered atomics - examples - https://www.youtube.com/watch?v=M15UKpNlpeM&ab_channel=CppCon

## Dokumentacja + slajdy

* https://infotraining.bitbucket.io/cpp-thd

## Środowisko (2 opcje do wyboru)

  1. Konfiguracja lokalna

     Jeśli uczestnik chce skonfigurować lokalnie swoje środowisko, to należy zainstalować:
     
     * Dowolny kompilator C++ (gcc, clang, Visual C++) wspierający C++11/14
     * [CMake > 3.15](https://cmake.org/)
       * proszę sprawdzić wersję w lini poleceń

         ```
         cmake --version
         ```
  
  2. Wirtualna maszyna - VirtualBox 6.1
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
