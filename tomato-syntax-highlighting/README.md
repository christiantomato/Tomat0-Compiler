# Tomat0 Syntax Highlighting 
Follow the steps below to enable syntax highlighting on vscode for .tmt files. The highlighting is purely for readability and cosmetics - there is no _syntax checking_. 

### Installing the Extension
To install the extension, simply make a symbolic link of this directory to your .vscode/extensions folder.
You can do this by running the following command while in this directory:
```
ln -s syntax-highlighting ~/.vscode/extensions/syntax-highlighting
```
Alternatively, you can just copy the folder (not recommended as you will have to re-copy if changes are made).
```
cp -r syntax-highlighting ~/.vscode/extensions
```

### Using the Custom Tomat0 Color Theme (Optional):
To use the custom tomat0 color theme, copy and paste the following into your 
vscode user settings json file. 

```json
//tomat0 customization.
"editor.tokenColorCustomizations": {
    "textMateRules": [
        {
            "scope": "keyword.control.tomat0",
            "settings": {
                "foreground": "#ff8f8f"
            }
        },
        {
            "scope": "string.quoted.double.tomat0",
            "settings": {
                "foreground": "#a8ff78"
            }
        },
        {
            "scope": "constant.numeric.tomat0",
            "settings": {
                "foreground": "#f8c555"
            }
        },
        {
            "scope": "comment.line.tomat0",
            "settings": {
                "foreground": "#666666",
                "fontStyle": "italic"
            }
        }
    ]
}
```