!insertmacro LANGFILE_EXT Japanese

!pragma warning disable 6030
${LangFileString} MUI_TEXT_WELCOME_INFO_TEXT "このセットアップは$(^NameDA)のインストールを案内します。$\r$\n$\r$\nインストールを開始する前に、Geometry Dashが実行されていないことを確認してください。$\r$\n$\r$\n$_CLICK"
${LangFileString} MUI_UNTEXT_WELCOME_INFO_TEXT "このセットアップは$(^NameDA)のアンインストールを案内します。$\r$\n$\r$\nアンインストールを開始する前に、Geometry Dashが実行されていないことを確認してください。$\r$\n$\r$\n$_CLICK"
!pragma warning default 6030

; installer

${LangFileString} SAPFIRE_TEXT_GD_MISSING "$\r$\n$\r$\nこのパスにはGeometry Dashがインストールされていません！"
${LangFileString} SAPFIRE_TEXT_GD_OLD "$\r$\n$\r$\nGeometry DashのバージョンはこのSapfireのバージョンには古すぎます!"
${LangFileString} SAPFIRE_TEXT_MOD_LOADER_ALREADY_INSTALLED "このパスには他のモッドがインストールされています！$\r$\nそれらはSapfireによって上書きされます。（the dll trademark）"

; uninstaller

${LangFileString} SAPFIRE_UNTEXT_SAPFIRE_MISSING "このパスにはSapfireがインストールされていません！"
