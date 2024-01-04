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
  





<div id="{$sectionId}"  class="plugin-section fullwidth">
  <span class="legend">
    {if !empty($sectionIcon)}<img src="{$sectionIcon|escape}" alt=""/>{/if}{$section|escape}
  </span>
  <div>

    
    <table style="border: 1px solid grey; width: 50%;"> 

    <thead style="border: 1px solid grey; width: 50%;">
      <tr><td style="border: 1px solid grey; width: 50%;">{t}Name{/t}</td>
      {foreach from=$attributes.ups_stats.dummy1.data key=item item=subitem}
        {foreach from=$subitem key=k item=value}
        <td style="border: 1px solid grey; width: 50%;"><b>{$k}</b></td>
        {/foreach}

      {/foreach}</tr>
    </thead>
    <tbody>
    {foreach from=$attributes.ups_stats item=ups}
      <tr>
      <td style="border: 1px solid grey; width: 50%;">{$ups.name}</td>
      {foreach from=$ups.data key=item item=subitem}
        {foreach from=$subitem key=k item=value}
          {if {$attributes.remoteCacheMappingDashboard.$item.$k} eq 'bar'}
            {if $value > 80}
                {assign var="color" value="#339933"}
            {else}
                {if $value > 50}
                    {assign var="color" value="#ff9933"}
                {else}
                    {assign var="color" value="#ff0000"}
                {/if}
            {/if}
            <td  style="border: 1px solid grey; width: 50%;">
            <div class="progress-bar-total">
              <div class="progress-bar-part" style="width:{$value}px; background: {$color}">{$value}%</div>
            </div>
            </td>
        {else}
          <td style="border: 1px solid grey; width: 50%;"><b>{t}{$value}{/t}</b></td>
        {/if}
          
          {/foreach}
        {/foreach} 
        </tr>
      {/foreach}
    
    
    
    </tbody>

    
    </table>
  </div>
</div>