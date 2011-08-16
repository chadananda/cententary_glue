Drupal.behaviors.travelmappicker = function(context) {
  $('#travelmap-picker:not(.travelmap-picker-processed)', context)
  .click(function(e){
    var x = e.pageX - $(this).offset().left;
    var y = e.pageY - $(this).offset().top;
    $('#edit-field-pos-x-0-value').val(x * 2);
    $('#edit-field-pos-y-0-value').val(y * 2);
    $('#travelmap-picker .travelmap-city').css({ 'top': y + 'px' }).css({ 'left': x+'px' }).show();
  })
  .addClass('travelmap-picker-processed');
  $('#edit-field-map-flag-type-value:not(.travelmap-picker-processed)', context)
  .change(function(e){
    var flagtype = $(this).val();
    $('#travelmap-picker .travelmap-city-marker').html('<img width="24" height="24" src="/sites/all/themes/centenary/images/' + flagtype + '.png">');
  })
  .addClass('travelmap-picker-processed');
}
