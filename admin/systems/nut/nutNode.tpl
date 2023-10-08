<div id="{$sectionId}" class="plugin-section fullwidth">
    <span class="legend">
        {if !empty($sectionIcon)}<img src="{$sectionIcon|escape}" alt="" />{/if}{$section|escape}
    </span>
</div>
    {if $attributes.fdNutInfos}
	{foreach from=$attributes.fdNutInfos key=element item=infos}

	<div id="{$element}" class="plugin-section">
	<span class="legend">{t}{$element}{/t}</span>
	<table>
		{foreach from=$infos key=value item=info}
	 	<tr>
                	<td style="border: 1px solid grey; width: 50%;"><b>{t}{$value}{/t}</b></td>
                <td style="width: 50%; border: 1px solid grey">

                {if $info.render eq 'bar'}
                        {include file='/usr/local/share/fusiondirectory/plugins/admin/systems/nut/bar.tpl'}
                {else}
                        {t}{$info.value}{/t}
                {/if}
                </td>
        	{/foreach}
	</table>


	</div>
	{/foreach}



    {/if}
