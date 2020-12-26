/**
 * 
 * @param {String} url 
 */
function gReq(method, url) {
    return new Promise(function (resolve, reject) {
        let xhr = new XMLHttpRequest();
        xhr.open(method, url);
        xhr.onload = function () {
            if (this.status >= 200 && this.status < 300) {
                resolve(xhr.response);
            } else {
                reject({
                    status: this.status,
                    statusText: xhr.statusText
                });
            }
        };
        xhr.onerror = function () {
            reject({
                status: this.status,
                statusText: xhr.statusText
            });
        };
        xhr.send();
    });
}

function gotError(txt) {
	console.trace("got error")
	document.getElementById("errorText").innerHTML = txt 
	document.getElementById("apiError").style.display = "block"
	document.getElementById("wackyLoading").remove()
}

function fadeOutPlaceholder() {
	var placeholder = document.getElementById("wackyLoading")
	placeholder.classList.remove("visible")
	placeholder.classList.add("hidden")
	setTimeout(function() { placeholder.remove() }, 1000 * 0.75)
}

var elementCount = 0;

function createSvg(d,cList = "repoSvg") {
	var svg = document.createElementNS("http://www.w3.org/2000/svg","svg")
	svg.classList = cList
	svg.setAttribute('viewbox', '0 0 24 24');

	var path = document.createElementNS("http://www.w3.org/2000/svg","path")
	path.setAttribute("d",d)
	svg.appendChild(path)

	return svg
}

function readableTime(t) {
	//thanks mozilla
	function pad(n){return n<10 ? '0'+n : n}

	var d = new Date(t)
	return d.getFullYear() + "-" + pad(d.getMonth()) + "-" + pad(d.getDay())
}

function constructMeta(project) {
	var container = document.createElement("div")
	container.classList = "meta"
	container.appendChild(createSvg("M12 .587l3.668 7.568 8.332 1.151-6.064 5.828 1.48 8.279-7.416-3.967-7.417 3.967 1.481-8.279-6.064-5.828 8.332-1.151z","svg-star"))
	container.innerHTML += "<span> " + project.stargazers_count + " </span> <span class=\"repo-lang\"><b>|</b> " + project.language + "</span>"

	return container
}

/**
 * 
 * @param {String} url 
 * @param {HTMLSpanElement} el 
 */
function getTags(url,el) {
	gReq("GET",url)
	.then(function(res) {
		el.classList.remove("self-loading")
		el.innerText = JSON.parse(res).map(t => t.name).join(", ")
	})
	.catch(function(err) {
		el.remove()
	})
}

function constructProject(project) {
	if(!project.description) return
	if(!project.description.includes(":listOnSite:")) return
	else project.description = project.description.replace(":listOnSite:","")

	console.log(project)
	var _parent = document.getElementById("projectContainer")
	var parent = document.createElement("a")
	parent.classList = "repoHref"
	parent.href = project.html_url

	var container = document.createElement("div")
	container.classList = "project hidden"

	var title = document.createElement("h1")
	var tags = document.createElement("span")
	tags.classList = " self-loading tags"
	tags.innerText = " loading"
	getTags(project.tags_url,tags)
	var description = document.createElement("p")
	title.innerText = project.name||"project has no name"
	title.appendChild(tags)
	description.innerText = project.description||"project has no description"

	container.appendChild(title)
	container.appendChild(description)
	container.appendChild(constructMeta(project))

	parent.appendChild(container)
	_parent.appendChild(parent)

	setTimeout(function() { 
		container.classList.remove("hidden")
		container.classList.add("visible")
	}, 1000 * 0.75 + 100 * elementCount) 
	elementCount++;
}

//https://api.github.com/users/USERNAME/repos
function getRepos() {
	var epic_path = "https://api.github.com/users/ffamilyfriendly/repos"
	gReq("GET", "https://api.github.com/users/ffamilyfriendly/repos")
	.then(function(res) {
		var jsonObj;
		try {
			jsonObj = JSON.parse(res)
		} catch(err) {
			return gotError("could not parse JSON: " + err)
		}

		fadeOutPlaceholder()
		for(var i = 0; i < jsonObj.length; i++) constructProject(jsonObj[i])
	})
	.catch(function(rejected) {
		gotError("<b>" + rejected.status + "</b> " + rejected.statusText)
	})
}

document.addEventListener("DOMContentLoaded", function() {
	getRepos()
})