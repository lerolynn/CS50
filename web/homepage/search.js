let past_visitors = [];
let counter = 0;

function visitors()
{
    let text = ""
    let name = document.querySelector('#name').value;

    for (var i = 0; i < past_visitors.length; i++)
    {
        text += past_visitors[i] + "<br>";
    }
    document.getElementById("past").innerHTML = text;

    if (name !== '')
    {
        counter++;
        document.querySelector('#result').innerHTML = "Welcome, " + name + "!";
        past_visitors.push(name);

    }
}