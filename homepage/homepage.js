document.addEventListener("DOMContentLoaded", function() {

    // plays audio when woofButton clicked
    let woofButton = document.getElementById("woofButton");
    if (woofButton != null) { // if button on page add eventlistener
        woofButton.addEventListener("click", function() { // will play woof audio when button clicked
            let audio = document.getElementById("woofAudio");
            playAudio(woofButton, audio);
        })
    }

    let meowButton = document.getElementById("meowButton");
    if (meowButton != null) { // if button on page add eventlistener
        meowButton.addEventListener("click", function() { // will play meow audio when button clicked
            let audio = document.getElementById("meowAudio");
            playAudio(meowButton, audio);
        })
    }

});

function playAudio(button, audio) { // plays audio
    audio.play();
}
