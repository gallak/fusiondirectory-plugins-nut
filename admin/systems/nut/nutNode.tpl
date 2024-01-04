{block name=head}
    <style>
    .progress-bar-total {
      width: 110px;
      max-width: 110px;
      border: #999 solid 2px;
     background: #ccc;
    }
    .progress-bar-part {
      padding: 2px 5px;
      text-align: right;
    }
    </style>
{/block}
    


<div id="{$sectionId}" class="plugin-section fullwidth">
    <span class="legend">
        {if !empty($sectionIcon)}<img src="{$sectionIcon|escape}" alt="" />{/if}{$section|escape}
    </span>
</div>
    {if $attributes.remoteData}
	{foreach from=$attributes.remoteData key=element item=infos}

	<div id="{$element}" class="plugin-section">
	<span class="legend">{t}{$element}{/t}</span>
   
	<table>
		{foreach from=$infos key=var item=value}
	 	<tr>
               	<td style="border: 1px solid grey; width: 50%;"><b>{t}{$var}{/t}</b></td>
                <td style="width: 50%; border: 1px solid grey">

                {if {$attributes.remoteCacheMapping.$element.$var} eq 'bar'}
                    {if $value > 80}
                        {assign var="color" value="#339933"}
                    {else}
                        {if $value > 50}
                            {assign var="color" value="#ff9933"}
                        {else}
                            {assign var="color" value="#ff0000"}
                        {/if}
                    {/if}
                    <div class="progress-bar-total">
                      <div class="progress-bar-part" style="width:{$value}px; background: {$color}">{$value}%</div>
                    </div>
                        
                {else}
                        {t}{$value}{/t}
                {/if}
                </td>
        	{/foreach}
	</table>


	</div>
	{/foreach}

    {else}

    no data!

    {/if}
