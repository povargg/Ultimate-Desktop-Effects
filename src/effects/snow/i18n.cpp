#include "i18n.h"
#include "../../settings_api.h"
namespace snowfx {
const wchar_t* const kLangCodes[LANG_COUNT]={L"EN",L"RU",L"UA",L"RO",L"DE"};
static const wchar_t* const kText[][LANG_COUNT]={
 // S_TRAY_TIP
 {L"Ultimate Desktop Effects: snow on your screen", L"Ultimate Desktop Effects: снег на экране", L"Ultimate Desktop Effects: сніг на екрані", L"Ultimate Desktop Effects: zăpadă pe ecran", L"Ultimate Desktop Effects: Schnee"},
 // S_WELCOME_TITLE
 {L"Ultimate Desktop Effects is running ❄", L"Ultimate Desktop Effects запущен ❄", L"Ultimate Desktop Effects запущено ❄", L"Ultimate Desktop Effects rulează ❄", L"Ultimate Desktop Effects läuft"},
 // S_WELCOME_TEXT
 {L"The snowflake lives in the tray. Left click opens settings, right click opens the menu.", L"Снежинка в трее. Левый клик открывает настройки, правый показывает меню.", L"Сніжинка в треї. Лівий клік відкриває налаштування, правий показує меню.", L"Fulgul stă în zona de notificare. Clic stânga deschide setările, clic dreapta deschide meniul.", L"Linksklick auf das Symbol: Einstellungen. Rechtsklick: Schnellmenü und Effektauswahl."},
 // S_SETTINGS_TITLE
 {L"Ultimate Desktop Effects: settings", L"Ultimate Desktop Effects: настройки", L"Ultimate Desktop Effects: налаштування", L"Ultimate Desktop Effects: setări", L"Ultimate Desktop Effects: Einstellungen"},
 // S_SUBTITLE
 {L"Holiday snow, drifts and blizzards right on your desktop", L"Новогодний снег, сугробы и метель прямо на рабочем столе", L"Новорічний сніг, замети та хуртовина просто на робочому столі", L"Zăpadă de sărbători, troiene și viscol direct pe desktop", L"Schneeflocken und Schneeverwehungen"},
 // S_CARD_SNOWFALL
 {L"Snowfall", L"Снегопад", L"Снігопад", L"Ninsoare", L"Schneefall"},
 // S_MODE_TOP
 {L"Over windows", L"Поверх окон", L"Поверх вікон", L"Peste ferestre", L"Vor Fenstern"},
 // S_MODE_BEHIND
 {L"Behind windows", L"За окнами", L"За вікнами", L"Sub ferestre", L"Hinter Fenstern"},
 // S_PRESET_LIGHT
 {L"Light", L"Лёгкий", L"Легкий", L"Ușoară", L"Leicht"},
 // S_PRESET_NORMAL
 {L"Normal", L"Обычный", L"Звичайний", L"Normală", L"Normal"},
 // S_PRESET_STRONG
 {L"Heavy", L"Сильный", L"Сильний", L"Puternică", L"Stark"},
 // S_PRESET_BLIZZARD
 {L"Blizzard", L"Метель", L"Хуртовина", L"Viscol", L"Schneesturm"},
 // S_INTENSITY
 {L"Intensity", L"Интенсивность", L"Інтенсивність", L"Intensitate", L"Schneestärke"},
 // S_FALL_SPEED
 {L"Fall speed", L"Скорость падения", L"Швидкість падіння", L"Viteza căderii", L"Fallgeschwindigkeit"},
 // S_WIND_ANGLE
 {L"Wind angle", L"Угол ветра", L"Кут вітру", L"Unghiul vântului", L"Windwinkel"},
 // S_GUSTS
 {L"Wind gusts", L"Порывы ветра", L"Пориви вітру", L"Rafale de vânt", L"Windböen"},
 // S_FLAKE_SIZE
 {L"Flake size", L"Размер снежинок", L"Розмір сніжинок", L"Mărimea fulgilor", L"Flockengröße"},
 // S_CARD_PILES
 {L"Drifts and cursor", L"Сугробы и курсор", L"Замети та курсор", L"Troiene și cursor", L"Schneeverwehungen"},
 // S_ACCUMULATION
 {L"Pile-up speed", L"Скорость накопления", L"Швидкість накопичення", L"Viteza acumulării", L"Ansammlung"},
 // S_PILE_TASKBAR
 {L"On the taskbar", L"У панели задач", L"Біля панелі завдань", L"Pe bara de activități", L"Schneehöhe: Taskleiste"},
 // S_PILE_WINDOWS
 {L"On windows", L"На окнах", L"На вікнах", L"Pe ferestre", L"Schneehöhe: Fenster"},
 // S_DRIFTS
 {L"Hills and bumps", L"Горки и неровности", L"Гірки та нерівності", L"Dealuri și denivelări", L"Hügel und Unebenheiten"},
 // S_MELT
 {L"Melting", L"Таяние", L"Танення", L"Topire", L"Schmelzen"},
 // S_CURSOR_RADIUS
 {L"Cursor radius", L"Радиус курсора", L"Радіус курсора", L"Raza cursorului", L"Mausradius"},
 // S_SMOOTHNESS
 {L"Smoothness", L"Плавность", L"Плавність", L"Fluiditate", L"Bildrate"},
 // S_CARD_BEHAVIOR
 {L"Behavior", L"Поведение", L"Поведінка", L"Comportament", L"Verhalten"},
 // S_SNOW_ON
 {L"Snow on", L"Снег включён", L"Сніг увімкнено", L"Zăpadă activată", L"Schnee einschalten"},
 // S_ON_WINDOWS
 {L"Pile snow on windows", L"Копить снег на окнах", L"Накопичувати сніг на вікнах", L"Adună zăpadă pe ferestre", L"Schnee auf Fenstern"},
 // S_ON_TASKBAR
 {L"Pile snow on the taskbar", L"Копить снег на панели задач", L"Накопичувати сніг на панелі завдань", L"Adună zăpadă pe bara de activități", L"Schnee auf der Taskleiste"},
 // S_CURSOR_KNOCK
 {L"Knock snow off with the cursor", L"Сбивать снег курсором", L"Збивати сніг курсором", L"Dărâmă zăpada cu cursorul", L"Schnee mit Maus wegfegen"},
 // S_CRYSTALS
 {L"Crystal flakes", L"Снежинки-кристаллы", L"Сніжинки-кристали", L"Fulgi de cristal", L"Kristallflocken"},
 // S_PAUSE_GAMES
 {L"Hide in games and fullscreen apps", L"Скрывать в играх и полноэкранных приложениях", L"Ховати в іграх і повноекранних програмах", L"Ascunde în jocuri și aplicații pe tot ecranul", L"In Spielen / Vollbild pausieren"},
 // S_AUTOSTART
 {L"Start with Windows", L"Запускать вместе с Windows", L"Запускати разом з Windows", L"Pornește odată cu Windows", L"Mit Windows starten"},
 // S_LANGUAGE
 {L"Language", L"Язык", L"Мова", L"Limbă", L"Sprache"},
 // S_SHAKE
 {L"Shake off snow", L"Стряхнуть снег", L"Струсити сніг", L"Scutură zăpada", L"Schnee abschütteln"},
 // S_DEFAULTS
 {L"Defaults", L"По умолчанию", L"За замовчуванням", L"Implicit", L"Standardwerte"},
 // S_DONE
 {L"Done", L"Готово", L"Готово", L"Gata", L"Fertig"},
 // S_MODE
 {L"Mode", L"Режим", L"Режим", L"Mod", L"Anzeigemodus"},
 // S_SNOWFALL_STRENGTH
 {L"Snowfall", L"Сила снегопада", L"Сила снігопаду", L"Intensitatea ninsorii", L"Schneefallstärke"},
 // S_SMOOTHNESS_FPS
 {L"Smoothness, FPS", L"Плавность, FPS", L"Плавність, FPS", L"Fluiditate, FPS", L"Bildrate (FPS)"},
 // S_SHAKE_ALL
 {L"Shake off all snow", L"Стряхнуть весь снег", L"Струсити весь сніг", L"Scutură toată zăpada", L"Allen Schnee abschütteln"},
 // S_ALL_SETTINGS
 {L"All settings…", L"Все настройки…", L"Усі налаштування…", L"Toate setările…", L"Alle Einstellungen…"},
 // S_EXIT
 {L"Exit", L"Выход", L"Вихід", L"Ieșire", L"Beenden"},
 // S_INSTALL_TITLE
 {L"Install Ultimate Desktop Effects", L"Установка Ultimate Desktop Effects", L"Встановлення Ultimate Desktop Effects", L"Instalare Ultimate Desktop Effects", L"Ultimate Desktop Effects installieren"},
 // S_UPDATE_TITLE
 {L"Update Ultimate Desktop Effects", L"Обновление Ultimate Desktop Effects", L"Оновлення Ultimate Desktop Effects", L"Actualizare Ultimate Desktop Effects", L"Ultimate Desktop Effects aktualisieren"},
 // S_INSTALL_SUB
 {L"Holiday snow on your screen", L"Новогодний снег на вашем экране", L"Новорічний сніг на вашому екрані", L"Zăpadă de sărbători pe ecranul tău", L"Schneeflocken und Schneeverwehungen"},
 // S_WHAT_INSTALLED
 {L"What gets installed", L"Что будет установлено", L"Що буде встановлено", L"Ce se instalează", L"Was installiert wird"},
 // S_INSTALL_DESC
 {L"Snowflakes fall over or behind your windows, pile up on windows and the taskbar, and the cursor can knock the snow off. The snowflake icon lives in the tray.", L"Снежинки падают поверх окон или за ними, копятся сугробами на окнах и панели задач, а курсором снег можно сбить. Значок снежинки живёт в трее.", L"Сніжинки падають поверх вікон або за ними, накопичуються заметами на вікнах і панелі завдань, а курсором сніг можна збити. Значок сніжинки живе в треї.", L"Fulgii cad peste ferestre sau în spatele lor, se adună în troiene pe ferestre și pe bara de activități, iar cursorul poate dărâma zăpada. Pictograma fulgului stă în zona de notificare.", L"Schneeflocken fallen vor oder hinter Fenstern und bilden Verwehungen auf Fenstern und Taskleiste. Von povargg und flipkips (MIT)."},
 // S_FOLDER
 {L"Folder: ", L"Папка: ", L"Папка: ", L"Dosar: ", L"Ordner: "},
 // S_OPTIONS
 {L"Options", L"Параметры", L"Параметри", L"Opțiuni", L"Optionen"},
 // S_DESKTOP_LINK
 {L"Desktop shortcut", L"Ярлык на рабочем столе", L"Ярлик на робочому столі", L"Comandă rapidă pe desktop", L"Desktop-Verknüpfung"},
 // S_RUN_AFTER
 {L"Launch after install", L"Запустить после установки", L"Запустити після встановлення", L"Pornește după instalare", L"Nach der Installation starten"},
 // S_CANCEL
 {L"Cancel", L"Отмена", L"Скасувати", L"Anulează", L"Abbrechen"},
 // S_INSTALL
 {L"Install", L"Установить", L"Встановити", L"Instalează", L"Installieren"},
 // S_UPDATE
 {L"Update", L"Обновить", L"Оновити", L"Actualizează", L"Aktualisieren"},
 // S_COPY_FAILED
 {L"Could not copy the file to\n", L"Не удалось скопировать файл в папку\n", L"Не вдалося скопіювати файл у папку\n", L"Fișierul nu a putut fi copiat în\n", L"Datei konnte nicht kopiert werden nach\n"},
 // S_INSTALL_ERROR
 {L"Installation error", L"Ошибка установки", L"Помилка встановлення", L"Eroare la instalare", L"Installationsfehler"},
 // S_INSTALL_DONE
 {L"Done! You can start Ultimate Desktop Effects from the Start menu.", L"Готово! Ultimate Desktop Effects можно запустить из меню «Пуск».", L"Готово! Ultimate Desktop Effects можна запустити з меню «Пуск».", L"Gata! Poți porni Ultimate Desktop Effects din meniul Start.", L"Fertig! Ultimate Desktop Effects lässt sich im Startmenü öffnen."},
 // S_UNINSTALL_TITLE
 {L"Uninstall Ultimate Desktop Effects", L"Удаление Ultimate Desktop Effects", L"Видалення Ultimate Desktop Effects", L"Dezinstalare Ultimate Desktop Effects", L"Ultimate Desktop Effects deinstallieren"},
 // S_UNINSTALL_SUB
 {L"Remove the snow from this computer", L"Убрать снег с этого компьютера", L"Прибрати сніг із цього комп'ютера", L"Elimină zăpada de pe acest computer", L"Ultimate Desktop Effects entfernen"},
 // S_UNINSTALL_CARD
 {L"What will be removed", L"Что будет удалено", L"Що буде видалено", L"Ce se elimină", L"Was entfernt wird"},
 // S_UNINSTALL_DESC
 {L"The program, its Start menu and desktop shortcuts, and autostart.", L"Программа, её ярлыки в меню «Пуск» и на рабочем столе, автозапуск.", L"Програма, її ярлики в меню «Пуск» і на робочому столі, автозапуск.", L"Programul, comenzile rapide din meniul Start și de pe desktop, pornirea automată.", L"Die Anwendung, Verknüpfungen und der Autostart-Eintrag."},
 // S_DELETE_SETTINGS
 {L"Also delete my settings", L"Удалить и мои настройки", L"Видалити й мої налаштування", L"Șterge și setările mele", L"Auch meine Einstellungen löschen"},
 // S_UNINSTALL
 {L"Uninstall", L"Удалить", L"Видалити", L"Dezinstalează", L"Deinstallieren"},
 // S_REMOVED_TITLE
 {L"Ultimate Desktop Effects removed", L"Ultimate Desktop Effects удалён", L"Ultimate Desktop Effects видалено", L"Ultimate Desktop Effects a fost eliminat", L"Ultimate Desktop Effects wurde entfernt"},
 // S_REMOVED_TEXT
 {L"The snow has melted. Happy holidays! ❄", L"Снег растаял. С наступающим! ❄", L"Сніг розтанув. Зі святами! ❄", L"Zăpada s-a topit. Sărbători fericite! ❄", L"Ultimate Desktop Effects wurde entfernt."},
 // S_CLOSE
 {L"Close", L"Закрыть", L"Закрити", L"Închide", L"Schließen"},
 // S_UNINSTALL_LINK
 {L"Uninstall Ultimate Desktop Effects", L"Удалить Ultimate Desktop Effects", L"Видалити Ultimate Desktop Effects", L"Dezinstalează Ultimate Desktop Effects", L"Ultimate Desktop Effects deinstallieren"},
 // S_CURSOR_WIND
 {L"Wind follows cursor", L"Ветер за курсором", L"Вітер за курсором", L"Vântul după cursor", L"Wind folgt der Maus"},
 // S_SHOW_FALLING
 {L"Show falling snow", L"Показывать падающий снег", L"Показувати сніг, що падає", L"Arată zăpada care cade", L"Fallenden Schnee anzeigen"},
 // S_AUTHOR
 {L"by povargg & flipkips", L"Автор: povargg & flipkips", L"Автор: povargg & flipkips", L"Autor: povargg & flipkips", L"povargg & flipkips"},
 // S_STAR_GITHUB
 {L"★ Star it on GitHub", L"★ Поставьте звезду на GitHub", L"★ Поставте зірку на GitHub", L"★ Dă-i o stea pe GitHub", L"Originalprojekt auf GitHub"},
};
static_assert(sizeof(kText)/sizeof(kText[0])==S_COUNT,"translation count mismatch");
int CurrentLang(){return ::CurrentLang();}
const wchar_t* Tr(Str id){return kText[id][CurrentLang()];}
}
