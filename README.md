virt2real pins SDK
===========

Данный проект представляет собой SDK для управления пинами ввода вывода платы микроконтроллера virt2real. Подробности о данной плате можно найти на сайте разработчиков http://virt2real.ru, а также на Хабре по ссылке http://habrahabr.ru/company/virt2real/blog.

SDK реализован на языке C++ с использованием расширений C++0x. Сборка проектов из данного проводится с помощью среды разработки NetBeans 7.3.1. 

Перед тем как начать собирать SDK необходимо установить и сконфигурировать средства разработки. Для этого сначала выполним следующую команду:

	suso apt-get install make git qt4-qmake qt4-dev-tools libqt4-dev

Теперь установим NetBeans, если данная среда разработки не установлена. Сделать это можно скачав установочный файл с официального сайта https://netbeans.org. Для установки также может потребоваться JDK. При этом не рекомендуется использовать OpenJDK, входящий в дистрибутив Linux, поскольку с ним работа NetBeans может быть нестабильной. Лучше использовать JDK от Oracle.

После установки среды разработки необходимо установить toolchain для компиляции исходных кодов проекта. Для это требуется выполнить следующие команды:

	wget http://sourcery.mentor.com/public/gnu_toolchain/arm-none-linux-gnueabi/arm-2013.05-24-arm-none-linux-gnueabi.bin
	# С помощью этой команды можно сменить интерпретатор на который указывает /bin/sh с dash на bash
	# поскольку инсталятор toolchain'а не работает с dash.
	sudo dpkg-reconfigure -plow dash   # В открывшемся диалоге выбираем "Нет"
	bash arm-2013.05-24-arm-none-linux-gnueabi.bin   # Загружается графический инсталлятор. Далее следуем его инструкциям

После того, как toolchain установлен, необходимо сконфигурировать среду разработки, чтобы она могла его использовать для сборки проекта. Для этого открываем NetBeans, заходим в меню "Сервис->Параметры" и в появившемся окне переходим на вкладку "C/C++ -> Средства сборки". Теперь нам необходимо задать новый набор средств для компиляции файлов исходного кода под virt2real. Для этого нажимаем кнопку "Добавить" и выбираем базовый каталог как <каталог куда установлен toolchain>/bin. Затем выбираем набор средств. В нашем случае можно выбрать GNU, поскольку NetBeans не умеет автоматически распознавать CodeSourcery toolchain. Наконец задаем имя новому набору средств V2R и нажимаем OK.

Возвращаемся в окно настроек, выбираем в списке набор средств V2R и задаем исполняемые файлы компиляторов и сопуствующих программ следующим образом:

	Компилятор C: <базовый каталог>/arm-none-linux-gnueabi-gcc
	Компилятор C++: <базовый каталог>/arm-none-linux-gnueabi-g++
	Ассемблер: <базовый каталог>/arm-none-linux-gnueabi-as
	Команда отладки: <базовый каталог>/arm-none-linux-gnueabi-gdb

Все остальное можно оставить по умолчанию. Теперь все готово для загрузки и сборки SDK. В консоли выполняем следующие команды:

	mkdir ~/src
	cd ~/src
	git clone https://github.com/phoenix367/v2rpins_sdk

В появившемся каталоге v2rpins_sdk содержатся исходные коды самого SDK, а также демонстационной программы. Данный каталог 
имеет следующую структуру:

	pincontroller - каталог с иходным кодом SDK
	PpmDemo - каталог с проектом демонстрационной программы для управления сервоприводом
	LICENSE - файл лицензии
	README.md - файл readme
	.gdbinit - файл инициализации отладчика GDB


