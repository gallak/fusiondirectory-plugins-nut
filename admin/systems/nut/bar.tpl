
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

{if $info.value > 80}
	{assign var="color" value="#339933"}
{else}
	{if $info.value > 50}
		{assign var="color" value="#ff9933"}
	{else}
		{assign var="color" value="#ff0000"}
	{/if}
{/if}
<div class="progress-bar-total">
  <div class="progress-bar-part" style="width:{$info.value}px; background: {$color}">{$info.value}%</div>
</div>
