function validateemail() {
    var x = document.getElementById("email");
    var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;

        if (reg.test(x.value) == false) 
        {
            alert('Invalid Email Address');
            return false;
        }

        return true;
}

function validatesname(){
    var x = document.getElementById("servername");
    var m = document.getElementById("msg");
    var reg = /.*[A-Z].*\d.*|.*\d.*[A-Z].*/;
    if (reg.test(x.value) == false) {
        m.innerHTML="Invalid server name";
    }else{
        m.innerHTML="";
    }
}

function clearmsg() {
    var m = document.getElementById("msg");
    m.innerHTML="";
}

function checkPassword(){
    var psw = document.getElementById("psw");
    var cfpsw = document.getElementById("cfmpsw");
    if(psw.value != cfpsw.value){
        alert("The password doesnt match. Try again");
    }
}

function allowDrop(ev) {
    ev.preventDefault();
  }
  
  function drag(ev) {
    ev.dataTransfer.setData("text", ev.target.id);
  }
  
  function drop(ev) {
    ev.preventDefault();
    var data = ev.dataTransfer.getData("text");
    ev.target.appendChild(document.getElementById(data));
  }

  function keydown(e) {
    if (e.ctrlKey && e.keyCode == 77) {
        if (e.preventDefault) {
            e.preventDefault();
            var element = document.body;
            element.classList.toggle("dark-mode");
        }
        else {
            return false;
        }
    }
}

  function submitFunc(){
    var mn = document.getElementById("manname");
    if(mn.value=="") {
        alert("Enter Manager name");
        return;
    }  

    var ge = document.getElementById("email");
    var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
    if (reg.test(ge.value) == false) {
        alert('Invalid Email Address');
        return;
    }

    var su = document.getElementById("servername");
    reg = /.*[A-Z].*\d.*|.*\d.*[A-Z].*/;
    if (reg.test(su.value) == false) {
        alert("Invalid Server Username");
    }

    var p = document.getElementById("psw");
    if(p.value=="") {
        alert("Enter Password");
        return;
    }
    var cp = document.getElementById("cfmpsw");
    if(p.value!=cp.value){
        alert("Passwords doesnt match");
        return;
    }


    var tl = document.getElementById("tl");
    if(tl.value==""){
        alert("Choose Team Lead");
        return;
    }

    var div2 = document.getElementById("div2");
    div2.forEach(e => {
        console.log(e.value)
    });

    alert("Name: "+mn.value+"\n"+"Email: "+ge.value+"\n"+"Username: "+su.value+"\n"+"Team Lead: "+tl.value+"\n");

  }