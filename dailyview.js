Drupal.behaviors.dailyview = function(context) {
  var mainht = $(".view-display-id-attachment_4").height();
  var sideht = $(".view-display-id-attachment_2").height();
  if (mainht > sideht) {
    newht = (mainht - sideht) % 200;
    if (newht < 130) {
      $(".view-display-id-attachment_2").css('margin-bottom', newht + 'px');
    }
  }
  else if (sideht > mainht) {
    newht = (sideht - mainht) % 200;
    if (newht > 60) {
      $(".view-display-id-attachment_2").css('margin-bottom', newht + 'px');
    }
  }
}
