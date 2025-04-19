document.addEventListener("DOMContentLoaded", function() {

    const months = { // creates months in a way that can be swaped for another language
        en: ["January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"]
    };

    monthSelect = document.getElementById("months");

    let lang = "en";
    months[lang].forEach((month, index) => {
        const option = document.createElement("option");
        option.value = index + 1;
        option.textContent = month;
        monthSelect.appendChild(option);
    })
});
