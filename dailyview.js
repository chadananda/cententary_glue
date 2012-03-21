Drupal.behaviors.dailyview = function(context) {
  var mainht = $(".view-display-id-attachment_4").height();
  var sideht = $(".view-display-id-attachment_2").height();
  if (mainht > sideht) {
    htdiff = (mainht - sideht) % 200;
    if (mainht - sideht < 19) {
      $(".view-display-id-attachment_4").css('margin-bottom', htdiff + 'px');
    }
    else if (htdiff < 145) {
      $(".view-display-id-attachment_2").css('margin-bottom', htdiff + 'px');
    }
  }
  else {
    htdiff = (sideht - mainht) % 200;
    $(".view-display-id-attachment_2").css('margin-bottom', (200 - htdiff) + 'px');
  }
}
