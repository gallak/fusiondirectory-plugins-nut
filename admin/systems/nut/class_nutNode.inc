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



class nutServerSelectAttribute extends SelectAttribute
{
  public function getAllnutServers()
    {
    global $config;
    $ldap = $config->get_ldap_link();
    $allServers=array();
    $ldap->search('(objectClass=fdNutService)', ['dn','cn','description']);
    while ($uServer = $ldap->fetch()) {
        $allServers[$uServer['dn']]= array(     'nutCn' => $uServer['cn'][0],
                                                'nutDescription' => $uServer['description'][0],
                                        );
        }
    return($allServers);
  }


    function setParent (&$plugin)
    {
        parent::setParent($plugin);
        if (is_object($this->plugin)) {
            $this->updateChoices();
        }
    }

    function updateChoices ()
    {
        global $config;
        $serversName    = [];
        $serversDescription = [];
        foreach ($this->getAllnutServers() as $name => $content){
            $serversDescription[] = $content['nutCn']." ".$content['nutDescription'];
            $serversDn[] = $name;
            $this->setChoices($serversDn, $serversDescription);
        }
    }

}









class nutNode extends simplePlugin
{

  public $nutHandler;

  protected $displayHeader  = TRUE;
  protected $loaded = TRUE;


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



  public function refresh ()
  {
    global $config;
    $this->connectNutServer();

    if($this->attrs['fdNutAlias'][0]){
      $upsName=$this->attrs['fdNutAlias'][0];
    }else{
      $upsName=$this->attrs['cn'][0];
    }

    $nutInfos=$this->nutHandler->getNutAllInfos($upsName);
   
    var_dump($nutInfos);
    
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
            ],
          ],
      'nodeTrace' => [
        'class' => ['fullwidth'],
        'name'  => _('Node Trace'),
        'attrs' => [
             new FakeAttribute('fdNutDetails'),
            ],
          // 'template' => get_template_path('nutnode.tpl', TRUE, dirname(__FILE__)),
          ],

    ];
  }


  function __construct ($dn = NULL, $object = NULL, $parent = NULL, $mainTab = FALSE)
  {

  parent::__construct($dn, $object, $parent, $mainTab);
  if (isset($this->attrs['fdNutServerDn'])){
	    $currentServer=$this->attrs['fdNutServerDn'];
	    $this->refresh();
	  }
    $this->ignore_account = FALSE;  // affiche l'activation ou pas du plugin
  }



  function connectNutServer() {
    global $config;

    $ldap = $config->get_ldap_link();
    $ldap->cd($this->fdNutServerDn);
    $ldap->search('(objectClass=fdNutService)', ['fdNutServiceURL','fdNutCertPath','fdNutServiceTimeout']);
    $attrs = $ldap->fetch();

    $nURL = $attrs['fdNutServiceURL'][0];
    $nPath = $attrs['fdNutCertPath'][0];
    $nTimeout = $attrs['fdNutServiceTimeout'][0];

    $urlArray=parse_url($nURL);
    
    $this->nutHandler = new nutService();
    $this->nutHandler->nutConnect($urlArray['host'], $urlArray['port'],$nTimeout);

  }

}