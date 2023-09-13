<?php

/*
  This code is an addon to FusionDirectory (https://www.fusiondirectory.org/)
  Copyright (C) 2021 Antoine Gallavardin

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

class serviceNut extends simpleService
{
  static function plInfo (): array
  {
    return [
      'plShortName'   => _('NUT Service'),
      'plDescription' => _('NUT Service').' ('._('Services').')',
      'plIcon'        => 'geticon.php?context=applications&icon=urbackup&size=16',
      'plObjectClass' => ['fdNutService'],

      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }

  /*!
   *  \brief The main function : information about attributes
   */
  static function getAttributesInfo (): array
  {
    return  [
      'main' => [
        'name'  => _('NUT Service'),
        'attrs' => [
          new StringAttribute(
            _('URL'), _('URL to access the nut Service'),
            'fdNutServiceURL', TRUE
          ),
          new IntAttribute(
            _('Timeout'), _('Time out for accessing nut Server'),
            'fdNutServiceTimeout', FALSE,1,240
          ),
          new StringAttribute(
            _('Certificate Path'), _('Certificat path'),
            'fdNutServiceCertPath', FALSE
          ),        
          ]
      ],
    ];
  }
}
