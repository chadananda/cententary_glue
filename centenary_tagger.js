Drupal.behaviors.centenaryTagger = function(context) {
  $('.cg_term_delete a:not(.cg_term_delete-processed)', context)
  .click(function(){
    var clas = $(this).parent().attr('class').split(/\s+/);
    $('.cg_term_links.' + clas[1]).hide();
    $(this).hide();
    var saveIt = function (data) {
      if (data != true) {
        $('.cg_term_links.' + clas[1]).show();
        $(this).show();
      }
    }
    $.ajax({
      type: 'POST',
      url: this.href,
      dataType: 'json',
      success: saveIt,
      data: js=1,
    });
    return false;
  })
  .addClass('cg_term_delete-processed');
}
