
import { initializeApp } from 'https://www.gstatic.com/firebasejs/10.5.2/firebase-app.js';
import {
	getDatabase,
	ref,
	update,
	onValue,
} from 'https://www.gstatic.com/firebasejs/10.5.2/firebase-database.js';

import Chart from 'chart.js';

// Initialize Firebase
const firebaseConfig = {
    apiKey: "AIzaSyBFFyscZA31a09ptrLpEMPgJp6dBDglppk",
    authDomain: "rc-car-8e3cd.firebaseapp.com",
    databaseURL: "https://rc-car-8e3cd-default-rtdb.firebaseio.com",
    projectId: "rc-car-8e3cd",
    storageBucket: "rc-car-8e3cd.appspot.com",
    messagingSenderId: "545996566464",
    appId: "1:545996566464:web:d74d1c1982a9e14a2f6225"
};

firebase.initializeApp(firebaseConfig);

// Reference to your Firebase Realtime Database
const database = firebase.database();

// Update the HTML with real-time data
const speedElement = document.getElementById('speed');
const accXElement = document.getElementById('accX');
const accYElement = document.getElementById('accY');

const speedValueRef = ref(database, 'Sensor/speed');
const accXValueRef = ref(database, 'Sensor/accX');
const accYValueRef = ref(database, 'Sensor/accY');

// onValue(speedValueRef, (snapshot) => {
// 	const value = snapshot.val();
// 	console.log(`Speed: ${value}`);
// 	document.getElementById('speed').innerText = value;
// });

// onValue(accXValueRef, (snapshot) => {
// 	const value = snapshot.val();
// 	console.log(`AccX: ${value}`);
// 	document.getElementById('accX').innerText = value;
// });

// onValue(accYValueRef, (snapshot) => {
// 	const value = snapshot.val();
// 	console.log(`AccY: ${value}`);
// 	document.getElementById('accY').innerText = value;
// });

// Listen for changes in the 'Sensor' node
database.ref('Sensor').on('value', (snapshot) => {
    const data = snapshot.val();

    // Update HTML with new data
    speedElement.innerText = `Speed: ${data.speed || 'N/A'} m/s`;
    accXElement.innerText = `Acceleration X: ${data.accX || 'N/A'} m/s^2`;
    accYElement.innerText = `Acceleration Y: ${data.accY || 'N/A'} m/s^2`;
});