# Syntax Highlighting Instructions

### Enabling the syntax highlighting extension

mv into extensions...

### Using The Tomat0 Color Theme:
To use the custom tomat0 color theme, copy and paste the following into your 
vscode user settings (JSON). 

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