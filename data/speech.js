$(function () {
    if ('speechSynthesis' in window) {
        speechSynthesis.onvoiceschanged = function () {
            var $voicelist = $('#voices');

            if ($voicelist.find('option').length == 0) {
                speechSynthesis.getVoices().forEach(function (voice, index) {
                    var $option = $('<option>')
                        .val(index)
                        .html(voice.name + (voice.default ? ' (default)' : ''));

                    $voicelist.append($option);
                });

                $voicelist.material_select();
            }
        }

        $('#speak').click(function () {
            var text = $('#message').val();
            var msg = new SpeechSynthesisUtterance();
            var voices = window.speechSynthesis.getVoices();
            msg.voice = voices[$('#voices').val()];
            msg.rate = $('#rate').val() / 10;
            msg.pitch = $('#pitch').val();
            msg.text = text;

            msg.onend = function (e) {
                console.log('Finished in ' + event.elapsedTime + ' seconds.');
            };

            speechSynthesis.speak(msg);
        })
    } else {
        $('#modal1').openModal();
    }
});