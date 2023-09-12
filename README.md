# fusiondirectory-plugins-nut

This is a plugin for FusionDirectory that report NUT information form a nut service (see https://networkupstools.org/)

## Why this plugin ?

Just add NUT informations

## Todo

Nut information design


## How to install

`git https://github.com/gallak/fusiondirectory-plugins-nut.git
mv fusiondirectory-plugins-nut nut`

### Installation of ldap schema

`ldap-schema-manager -i ./nut/contrib/schema/nut-fd.schema`

### Installation of plugin

```fusiondirectory-setup --set-fd_home=/usr/local/share/fusiondirectory --install-plugins
Setting fd_home to /usr/share/fusiondirectory
Installing FusionDirectory's plugins
Where is your plugins archive or folder?:
<complete current path>            
Available plugins:
1:nut

fusiondirectory-setup --update-cache --update-locales
```

### usage



