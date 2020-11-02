var toastInitialized = false;

function FF_INIT() {
    toastInitialized = true
    var toastArea = document.createElement("div")
    toastArea.classList = "toastarea"
    toastArea.id = "toastarea"
    document.body.prepend(toastArea)
}

function FF(find) {
    if(!find || typeof find != "string" || find.length == 1) return
    if(find[0] == "#") {
        return document.getElementById(find.substr(1))
    } else if(find[0] == ".") {
        return document.getElementsByClassName(find.substring(1))
    } else {
        return document.getElementsByTagName(find)
    }
}

function FF_toast(header,body,fadeout = 1000 * 10) {
    if(!toastInitialized) FF_INIT()

    var toast = document.createElement("div")
    toast.classList = "toast surface"
    var toast_title = document.createElement("h2")
    toast_title.innerText = header
    toast.appendChild(toast_title)
    toast.innerHTML += body
    FF("#toastarea").appendChild(toast)

    setTimeout(function() {
        toast.classList.add("fadeout")
        setTimeout(function() {
            toast.remove()
        },1100)
    },fadeout)
}