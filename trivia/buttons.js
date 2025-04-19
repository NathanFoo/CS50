document.addEventListener("DOMContentLoaded", function() {

    let buttons = document.getElementsByClassName("buttons");

    for (let i = 0; i < buttons.length; i++) {
        let button = buttons[i];
        if (i === 4) {
            button.addEventListener('click', function() {
                correct(button);
            });
        }
        else {
            button.addEventListener('click', function() {
                wrong(button);
            });
        }
    }

    let ansButton = document.getElementById("AnsButton");
    ansButton.addEventListener('click', function() {
        check(ansButton);
    });

    function check(ansButton) {
        let ans = document.getElementById("name").value;
        if(ans == "yes") {
            ansButton.style.backgroundColor = "green";
            document.querySelector('a').innerHTML = "Correct";
        }
        else {
            ansButton.style.backgroundColor = "red";
            document.querySelector('a').innerHTML = "Incorrect";
        }
    }

    function wrong(button) {
        let response = document.querySelector('p');
        response.innerHTML = "Incorrect";

        for (let i = 0; i < buttons.length; i++) {
            let b = buttons[i];
            if(b === button) {
                b.style.backgroundColor = "red";
            }
            else {
                b.style.backgroundColor = "#d9edff"
            }
        }
        console.log('wrong');
    }

    function correct(button) {
        let response = document.querySelector('p');
        response.innerHTML = "Correct!";

        for (let i = 0; i < buttons.length; i++) {
            let b = buttons[i];
            if(b === button) {
                b.style.backgroundColor = "green";
            }
            else {
                b.style.backgroundColor = "#d9edff";
            }
        }

        console.log('correct');
    }
});



