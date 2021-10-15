
function quayPhai() {
    //var quayPhai = document.getElementById("quayPhai");
    socket.send("1");
}
function quayTrai() {
    //var quayPhai = document.getElementById("quayPhai");
     socket.send("2");
}
function tienLen() {
    //var quayPhai = document.getElementById("quayPhai");
     socket.send("3");
     console.log("anhyeuem")
    
}
function diLui() {
    //var quayPhai = document.getElementById("quayPhai");
    socket.send("4");
}
function dungLai() {
    //var quayPhai = document.getElementById("quayPhai");
    socket.send("5");
}
function thayDoiTocDo() {
    var rangeTocDo = document.getElementById("tocDo");//khai báo biến 
    var valueTocDo = document.getElementById("giaTriTocDo");
    valueTocDo.innerHTML = rangeTocDo.value;
    
    socket.send("tocdo=" + rangeTocDo.value);
}