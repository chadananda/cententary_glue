/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


Drupal.behaviors.adjustNodes = function(context) {
  $('div.cg-adjust-properties:not(.cg-adjust-properties-processed)', context)
  .click(function(){
    var elem = $(this);
    var clas = $(this).attr('class').split(/\s+/);
    var nid = clas[0].replace('node-', '');
    var prop = clas[1];
    elem.toggleClass('Yes');
    var url = '/admin/centenary-adjust/' + nid + '/' + prop + '/' + $(this).attr('alt')
    var saveIt = function (data) {
      if (data == 'Yes') {
        elem.addClass('Yes');
      }
      else if (data == 'No') {
        elem.removeClass('No');
      }
      else if (data == 'error') {
        elem.toggleClass('Yes');
        alert('An error has occurred and your change could not be saved. Please reload the page \nand try again, and if this error contiunes, tell the maintenance team.')
      }
    }
    $.ajax({
      type: 'POST',
      url: url,
      dataType: 'json',
      success: saveIt,
      data: js=1
    });
    return false;
  })
  .addClass('cg-adjust-properties-processed');
}
