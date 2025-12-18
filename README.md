## If you want to create a mod based on this project you should:

Modify CMakeLists.txt with your name and the name of your mod
```cmake
set(AUTHOR_NAME "AuthorName")
set(PRODUCT_NAME "SKSEMenuFrameworkExample")
set(BEAUTIFUL_NAME "SKSE Menu Framework Example")
```

Delete the LICENCE file and use git to create the licence you want your mod to be

![Image 1](https://gist.github.com/user-attachments/assets/d8720331-4628-47e2-9493-b23b44adf644)
![Image 2](https://gist.github.com/user-attachments/assets/6a0ee652-5033-4c09-a65e-291e2b70be22)

Update the cmake/version.rc.in with your licence (This licence will go into the dll)

## Environment variables

[How to set up envioriment variables](https://gist.github.com/Thiago099/b45ec7832fb754325b29a61006bcd10c)

COMMONLIB_SSE_FOLDER

Clone this Repository, to somewhere safe and adds its path to this environment variable on Windows.

```bash
git clone --recursive https://github.com/alandtse/CommonLibVR
cd CommonLibVR
git checkout ng
```
  
## Optional ouput folder optional variables

- SKYRIM_FOLDER
- WILDLANDER_OWRT_FOLDER
- SKYRIM_OWRT_FOLDER
- SKYRIM_MODS_FOLDER2
- SKYRIM_MODS_FOLDER


## Description of the new features

https://github.com/QTR-Modding/SKSE-Menu-Framework-3/blob/master/README.md
