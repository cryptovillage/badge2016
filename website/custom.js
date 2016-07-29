function on_color_change(color) {
  frames[frame][0] = $(".led1 .led-input").spectrum("get").toHexString();
  frames[frame][1] = $(".led2 .led-input").spectrum("get").toHexString();
  frames[frame][2] = $(".led3 .led-input").spectrum("get").toHexString();
  frames[frame][3] = $(".led4 .led-input").spectrum("get").toHexString();
  frames[frame][4] = $(".led5 .led-input").spectrum("get").toHexString();
  frames[frame][5] = $(".led6 .led-input").spectrum("get").toHexString();
  frames[frame][6] = $(".led7 .led-input").spectrum("get").toHexString();
  frames[frame][7] = $(".led8 .led-input").spectrum("get").toHexString();
}

var frame = 0;
var repeat = false;
var timer;

var frames = [["#000000","#000000","#000000","#000000","#000000","#000000","#000000","#000000"]];
function play_animation() {
  $(".control-paused").show();
  $(".control-playing").hide();
  $(".led-input").spectrum("disable");
  console.log(beat_ms_delay());
  timer = setInterval(animation_step_forward, beat_ms_delay());
}

function pause_animation() {
  $(".control-paused").hide();
  $(".control-playing").show();
  $(".led-input").spectrum("enable");
  clearInterval(timer);
}

function bpm() {
  return parseInt($("#bpm").val());
}

function beat_ms_delay() {
  return 60000.0 / bpm(); 
}

function update_slider() {
  var slider = $("#slider");
  slider.attr("max", frames.length - 1);
  slider.val(frame);
  $("#slider-value").text(frame + 1);
  $("#slider-total").text(frames.length);
}

function animation_render_frame() {
  update_slider();
  $(".led1 .led-input").spectrum("set", frames[frame][0]);
  $(".led2 .led-input").spectrum("set", frames[frame][1]);
  $(".led3 .led-input").spectrum("set", frames[frame][2]);
  $(".led4 .led-input").spectrum("set", frames[frame][3]);
  $(".led5 .led-input").spectrum("set", frames[frame][4]);
  $(".led6 .led-input").spectrum("set", frames[frame][5]);
  $(".led7 .led-input").spectrum("set", frames[frame][6]);
  $(".led8 .led-input").spectrum("set", frames[frame][7]);
}

function animation_step_forward() {
  frame += 1;
  if (frame === frames.length) {
    frame = (repeat ? 0 : frames.length - 1);
  }
  animation_render_frame();
}

function animation_step_backwards() {
  frame -= 1;
  if (frame < 0) {
    frame = (repeat ? frames.length - 1 : 0);
  }
  animation_render_frame();
}

function add_frame() {
  frames.splice(frame,0,frames[frame].slice(0));
  animation_step_forward();
}

function toggle_repeat() {
  repeat = !repeat;
}

function slider_change_handler() {
  frame = parseInt($("#slider").val());
  animation_render_frame();
}

$(function() {
  $(".led-input").spectrum({
      preferredFormat: "hex",
      showInput: true,
      showPalette: true,
      localStorageKey: "spectrum.history",
      change: on_color_change,
      palette: [
          ['red', 'green', 'blue']
      ]
  });
  update_slider();
  $("#slider").change(slider_change_handler);
  $(".control-plus").click(add_frame);
  $(".control-back").click(animation_step_backwards);
  $(".control-forward").click(animation_step_forward);
  $(".control-repeat").click(toggle_repeat);
  $(".control-play").click(play_animation);
  $(".control-pause").click(pause_animation);
});

