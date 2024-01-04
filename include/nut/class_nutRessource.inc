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


class nutRessource {


  public $handle="";
  public $nutobj=array();
  private $serverRessourceDn="";
  private $item="";
  //private $nutData="";

  private $object="fdNutService";
  private $attributes=array('fdNutServiceURL','fdNutCertPath','fdNutServiceTimeout');


  public function __construct (string $server, string $item){
    $this->serverRessourceDn=$server;
    $this->item = $item;
  }


  function getRessourcesParameters() {
    global $config;

    $ldap = $config->get_ldap_link();
    $ldap->cd($this->serverRessourceDn);
    $ldap->search('(objectClass='.$this->object.')',$this->attributes);
    $attrs = $ldap->fetch();

    $URL = $attrs['fdNutServiceURL'][0];
    $Path = $attrs['fdNutCertPath'][0];
    $Timeout = $attrs['fdNutServiceTimeout'][0];

    $urlArray=parse_url($URL);
    
    return(array('host'=>$urlArray['host'], 'port' =>  $urlArray['port'],$Timeout ));
  }

  public function refreshRessource ()
  {
    global $config;
    // connection to the remote ressource
    $this->connect($this->getRessourcesParameters());
    // get remote ressource and format it in comprehensive array
    return($this->formatData($this->getOneRscData($this->item)));
  }

  // paramter : $ressourceConfig est un array
  // out : handle surla ressource 
  // connecty to remote source
  public function connect ($rscConfig){
    $errno="";
    $errstr="";
    $fp=fsockopen( $rscConfig['host'], $rscConfig['port'], $errno, $errstr, $rscConfig['timeout']);

    if (!$fp){
      msg_dialog::display(_('Error while connecting to Nut service'), "Error while getting informations :".$errstr ,INFO_DIALOG);
    }else{
      $this->handle=$fp;
    }
  
  }

  // Get data of one Ressource from remote source
  public function getOneRscData (string $item){

    $nutMsg = "LIST VAR ".$item."\nlogout";
    $nutEnd = "END";
    fwrite($this->handle, $nutMsg);
    while (!feof($this->handle)) {
      $line = trim(fgets($this->handle, 128));
      if (substr($line, 0, 3) == $nutEnd ){
        break;
      }
      if(substr($line, 0, 3) == 'VAR' ) {
        $info=preg_split("/ /",$line);
        $this->nutobj[$info[2]]=implode(" ",array_splice($info,3));
      }
      if(substr($line, 0, 3) == 'ERR' ) {
        $info=preg_split("/ /",$line);
        fwrite($this->handle, "logout");
        msg_dialog::display(_('Error while fetching info'), "Error while getting informations :".$info[1]." ".$ups ,INFO_DIALOG);
        break;
      }
    }
    fclose($this->handle);
    return($this->nutobj);
  }


// transform received data to a comprehensive array ready to used with RenderingMap
private function formatData($obj){
  $nutDataArray=array();
  // format received data to an comprehensive  
  foreach ($obj as $key => $val){
      $itemArray=explode(".",$key,2);
      $nutDataArray[$itemArray[0]][$itemArray[1]]=str_replace('"', "", $val);
  }
  return $nutDataArray;
}

  public function renderOneRsc ($data,$renderMap){
    $intersect=array_intersect_key(array_keys($data),array_keys($renderMap));
  
    $dataRendered=array();
    foreach ($intersect as $i){
        foreach ($data[$i] as $k => $v){
          if (isset($renderMap[$i][$k])){
            $dataRendered[$i][$k] = $v;
          }
        }
    }

    return($dataRendered);

  }

}

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
