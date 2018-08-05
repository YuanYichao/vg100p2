from flask import Flask, render_template, redirect, request
from flask_wtf import Form
from wtforms import StringField, IntegerField, FloatField, TextAreaField, ValidationError
from wtforms import validators
import sender
import asyncio
import judge


def validate_function(form, field):
    pass


def validate_script(form, field):
    pass


def validate_precision(form, field):
    pass


def validate_range(form, field):
    pass


def validate_coList(form, field):
    pass


def validate_varList(form, field):
    pass


class MyForm(Form):
    function = StringField('Input your function here',
                           validators=[validators.DataRequired()])
    varlist = StringField('What are the variables?', validators=[
        validators.required()], default= "x,y,z")
    colist = StringField('What are the coeifficents?', validators=[
        validators.required()], default="t")
    precision = FloatField('The unit of the following range',
                           validators=[validators.required()], default = 1)
    xstart = IntegerField('Variable 1 begins at', validators=[
                          validators.required()], default = -2)
    xend = IntegerField('Variable 1 ends at',
                        validators=[validators.required()],default = 2)
    ystart = IntegerField('Variable 2 begins at', validators=[
                          validators.required()],default = -2)
    yend = IntegerField('Variable 2 ends at',
                        validators=[validators.required()], default = 2)
    zstart = IntegerField('Variable 3 begins at', validators=[
                          validators.required()],default = -2)
    zend = IntegerField('Variable 3 ends at',
                        validators=[validators.required()], default = 2)
    script = TextAreaField('Script', validators=[validators.DataRequired()],default="var t; t = 0; start; t = sin(time()); back;")


def genCorrespondingObj(form):
    pass


def genSampleObj(function):
    r = {}
    r['xstart'] = -2
    r['ystart'] = -2
    r['zstart'] = -2
    r['xend'] = 2
    r['yend'] = 2
    r['zend'] = 2
    r['precision'] = 1
    r['function'] = function
    r['script'] = 'var t; t = 0; start; t = sin(time()); back;'
    r['varlist'] = ['x', 'y', 'z']
    r['colist'] = ['t']
    return r


app = Flask(__name__)


@app.route('/ex1')
def exampleone():
    obj = genSampleObj(
        "z - ((20 / (abs(x * x + y * y) + (20 / 4))) *  sin((x * x + y * y - t)* 1.0)) /5.0")
    sender.sender(obj, asyncio.new_event_loop())
    return redirect('/success')


@app.route('/ex2')
def exampletwo():
    obj = genSampleObj("sin(z) * x * t - z * y")
    sender.sender(obj, asyncio.new_event_loop())
    return redirect('/success')


@app.route('/ex3')
def examplethree():
    obj = genSampleObj(
        "sin(z) * z * x * t * cos(t) - z * cos(x) * y * sin(t) * sin(y * t)")
    sender.sender(obj, asyncio.new_event_loop())
    return redirect('/success')


@app.route('/ex4')
def exampleforth():
    obj = genSampleObj("sin(z) * x * t - z * z * cos(y)")
    sender.sender(obj, asyncio.new_event_loop())
    return redirect('/success')


@app.route('/ex5')
def examplefifth():
    obj = genSampleObj("z -  cos( sqrt( x * x + y * y ) ) - t")
    sender.sender(obj, asyncio.new_event_loop())
    return redirect('/success')


@app.route('/ex6')
def examplesixth():
    obj = genSampleObj("x * x  + y * y / 3 * 4  - z * z  * 2  - t")
    sender.sender(obj, asyncio.new_event_loop())
    return redirect('/success')


@app.route('/')
def index():
    return render_template('index.html', name="index")


@app.route('/samples')
def samples():
    return render_template('samples.html', name="samples")


@app.route('/success')
def success():
    return render_template('success.html', name="success")


@app.route('/customize', methods=('GET', 'POST'))
def customize():
    form = MyForm()
    print(form.validate_on_submit())
    if form.validate_on_submit():
        obj = request.form.to_dict()
        del obj['csrf_token']
        obj['varlist'] = obj['varlist'].split(',')
        obj['colist'] = obj['colist'].split(',')
        obj['xstart'] = int(obj['xstart'])
        obj['ystart'] = int(obj['ystart'])
        obj['zstart'] = int(obj['zstart'])
        obj['xend'] = int(obj['xend'])
        obj['yend'] = int(obj['yend'])
        obj['zend'] = int(obj['zend'])
        obj['precision'] = float(obj['precision'])
        if(not judge.judge(obj['function'], obj['colist'] + obj['varlist'])):
            return render_template('customize.html', form=form)
        else:
            print("ok")
        sender.sender(obj, asyncio.new_event_loop())
        return redirect('/success')
    return render_template('customize.html', form=form)


if __name__ == "__main__":
    app.config['SECRET_KEY'] = 'vg100'
    app.run(host="0.0.0.0", port=80)
