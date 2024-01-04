<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2016-2017  FusionDirectory
  Copyright (C) 2021-2023  Antoine Gallavardin

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */


class nutNode extends simpleRemotePlugin
{

  public $remoteHandler;
  public $remoteRessource="nut";
  public $remoteRenderingMap=array();

  protected $displayHeader  = TRUE;
  protected $loaded = TRUE;
  public $remoteData;
  
  static function plInfo (): array
  {
    return [
      'plShortName'   => _('Nut'),
      'plDescription' => _('Nut'),
      'plPriority'    => 30,
      'plObjectClass' => ['fdNutNode'],
      'plObjectType'  => ['component'],
      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }


  function refresh(){
    // Fetch remote and ordered data
    $tmpdata=$this->remoteHandler->refreshRessource();
    // Filter data base on Rendering Map and assign type
    $this->remoteData=$this->remoteHandler->renderOneRsc($tmpdata,$this->remoteRenderingMap);
    // Set Data for dislaying
    $this->attributesAccess['remoteData']->setValue($this->remoteData);
    // update Cache
    $this->updateRemoteObjectCache($this->remoteData);
    // save it
    $this->save();
  }

  static function getAttributesInfo (): array
  {
    return [
      'nodeConfig' => [
        'class' => ['fullwidth'],
        'name'  => _('Node Config'),
        'attrs' => [
            new nutServerSelectAttribute (_('Nut Service'),_('Name of server where the client is connected to'),
                'fdNutServerDn',TRUE),
            new StringAttribute(
                  _('Alias'), _('Alias UPS'),
                  'fdNutAlias', FALSE
                ),
            new ButtonAttribute(_('Refresh Informations'), _('Refresh informations'),
            'fdNutUpdateButton', _('Refresh'),'refresh'
            ),
            new HiddenAttribute('remoteCacheAction'),
            
          ],
          ],
      'nodeTrace' => [
        'class' => ['fullwidth'],
        'name'  => _('Unit Power Supply Info'),
        'attrs' => [
          new FakeAttribute('remoteCacheMapping'),
            new FakeAttribute('remoteData'),
            ],
           'template' => get_template_path('nutNode.tpl', TRUE, dirname(__FILE__)),
          ],

    ];
  }



  function __construct ($dn = NULL, $object = NULL, $parent = NULL, $mainTab = FALSE)
  {

    parent::__construct($dn, $object, $parent, $mainTab);
    
    $this->attributesAccess['remoteCacheAction']->setInLdap(FALSE);
   // $this->attributesAccess['remoteCacheMapping']->setInLdap(FALSE);
    $ressource=$this->remoteRessource."Ressource";
    $this->remoteRenderingMap=$this->convertTransformMap($this->remoteRessource);
    if (isset($this->attrs['fdNutServerDn'])){
        if($this->attrs['fdNutAlias'][0]){
          $upsName=$this->attrs['fdNutAlias'][0];
        }else{
          $upsName=$this->attrs['cn'][0];
        }

	       $currentServer=$this->attrs['fdNutServerDn'][0];
         $this->remoteHandler=new $ressource($currentServer,$upsName);
         // test mappig externe
         
         
         //var_dump($this->remoteRenderingMap);
         $this->attributesAccess['remoteCacheMapping']->setValue($this->remoteRenderingMap);
         if ( ! $this->remoteData ){
          $this->remoteData=$this->getRemoteObjectCache();
         }
    }

    $this->ignore_account = FALSE;  // affiche l'activation ou pas du plugin
  }

}
