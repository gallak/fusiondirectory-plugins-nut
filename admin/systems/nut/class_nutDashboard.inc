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


class nutDashboard extends simpleRemotePlugin
{

  //public $remoteHandler;
  //public $remoteRessource="nut";
  //public $remoteRenderingMap=array();

  protected $displayHeader  = FALSE;
  protected $loaded = TRUE;
  public $remoteData;

  private $render=array();
  
  static function plInfo (): array
  {
    return [
      'plShortName'   => _('UPS Reporting'),
      'plDescription' => _('UPS Reporting'),
      'plPriority'    => 30,
      'plObjectType'  => ['zone'],
      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }



  

  public function computeUpsStats(){
    global $config;
    $allElements=array();
    // get only allowed  field
  
    $this->remoteCacheMappingDashboard=$this->convertTransformMapDashbord("nut");
    $ldap = $config->get_ldap_link();
    $ldap->search('(&(fdDcimComponentZoneDn='.$this->dn.')(objectClass=fdNutNode))',['cn','description','ip','fdCacheData']);
    
    while ($element = $ldap->fetch()) {
      //get data cached
      $data = unserialize(gzinflate($element['fdCacheData'][0]));
      // keep only necessary item
      $intersect=array_intersect_key(array_keys($data),array_keys($this->remoteCacheMappingDashboard));
      $dataRendered=array();
      foreach ($intersect as $i){
          foreach ($data[$i] as $k => $v){
            if (isset($this->remoteCacheMappingDashboard[$i][$k])){
              $dataRendered[$i][$k] = $v;
            }
          }
      }
      // build all necessary data
        $allElements[$element['cn'][0]]=array(
          'description' => $element['description'][0],
          'name' => $element['cn'][0],
          'data' => $dataRendered
       );
   }

    return($allElements);


  }

  static function getAttributesInfo (): array
  {
    return [
        'upstats' => [
          'class' => ['fullwidth'],
          'name'  => _('Ups statistics'),
          'attrs' => [
            new FakeAttribute('ups_stats'),
            new FakeAttribute('remoteCacheMappingDashboard'),
          ],
          'template' => get_template_path('nutStat.tpl', TRUE, dirname(__FILE__)),
        ],
    ];
  }



  function __construct ($dn = NULL, $object = NULL, $parent = NULL, $mainTab = FALSE)
  {
    parent::__construct($dn, $object, $parent, $mainTab);    
    $this->ups_stats  = $this->computeUpsStats();
    $this->ignore_account = TRUE;
  }

}
