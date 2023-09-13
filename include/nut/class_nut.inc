<?php
class nutService {

    public $handle="";
    public $nutobj=array();

    function nutConnect(string $host = '127.0.0.1', string $port = '3493', int $timeout = 30)
    {
        $errno="";
        $errstr="";
        $fp=fsockopen( $host, $port, $errno, $errstr, $timeout);

        if (!$fp){
                echo "ERROR: $errno - $errstr";
                msg_dialog::display(_('Error while connecting to Nut service'), "Error while getting informations :".$errstr ,INFO_DIALOG);
        }else{
                $this->handle=$fp;
        }
    }

    function getNutAllInfos(string $ups){
        $nutMsg = "LIST VAR ".$ups."\nlogout";
        $nutEnd = "END";
        fwrite($this->handle, $nutMsg);
        while (!feof($this->handle)) {
            $line = trim(fgets($this->handle, 128));
            if (substr($line, 0, 3) == $nutEnd ){
                break;
            }
            if(substr($line, 0, 3) == 'VAR' ) {
                $info=preg_split("/ /",$line);
                $this->nutobj[$info[2]]=$info[3];
            }
        }
        fclose($this->handle);
        return($this->nutobj);
    }

    function getNutInfos(string $item){
        return($this->nutobj[$item]);
    }

}

?>
