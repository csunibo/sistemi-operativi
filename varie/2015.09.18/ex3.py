# Scrivere un programma che trovi fra tutti i file all'interno di un sottoalbero tutti I link simbolici che puntano a file o
# directory inesistenti.
import os

# print(f'{fullPath} (which pointed to {os.path.realpath(fullPath)}), has a fake reference')


def check(fullPath):
    resolved_path = os.path.realpath(fullPath)
    if os.path.exists(resolved_path) == False:
        print(f"{fullPath} -> {resolved_path}")


directory = "."
for walkingDirPath, subDirs, files in os.walk(directory):
    for fileName in files:
        fullPath = os.path.join(walkingDirPath, fileName)  # path completa
        if os.path.islink(fullPath):
            check(fullPath)
